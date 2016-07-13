/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimphuechromaconfig.c
 * Copyright (C) 2007 Michael Natterer <mitch@gimp.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* All references and functions in the code below that refer to "saturation"
 * actually use "LCH chroma" rather than "HSL saturation".
 * The UI says "chroma".
 * The code that allows to modify a range of colors has been disabled.
 * */

#include "config.h"

#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gegl.h>

#include "libgimpconfig/gimpconfig.h"

#include "operations-types.h"

#include "gimplchhuechromaconfig.h"

#include "gimp-intl.h"


 enum
 {
   PROP_0,
   PROP_RANGE,
   PROP_LIGHTNESS,
   PROP_CHROMA,
   PROP_HUE,
   PROP_OVERLAP
 };


static void     gimp_hue_chroma_config_iface_init   (GimpConfigInterface *iface);

static void     gimp_hue_chroma_config_get_property (GObject          *object,
                                                         guint             property_id,
                                                         GValue           *value,
                                                         GParamSpec       *pspec);
static void     gimp_hue_chroma_config_set_property (GObject          *object,
                                                         guint             property_id,
                                                         const GValue     *value,
                                                         GParamSpec       *pspec);

static gboolean gimp_hue_chroma_config_serialize    (GimpConfig       *config,
                                                         GimpConfigWriter *writer,
                                                         gpointer          data);
static gboolean gimp_hue_chroma_config_deserialize  (GimpConfig       *config,
                                                         GScanner         *scanner,
                                                         gint              nest_level,
                                                         gpointer          data);
static gboolean gimp_hue_chroma_config_equal        (GimpConfig       *a,
                                                         GimpConfig       *b);
static void     gimp_hue_chroma_config_reset        (GimpConfig       *config);
static gboolean gimp_hue_chroma_config_copy         (GimpConfig       *src,
                                                         GimpConfig       *dest,
                                                         GParamFlags       flags);


G_DEFINE_TYPE_WITH_CODE (GimpHueChromaConfig, gimp_hue_chroma_config,
                         GIMP_TYPE_SETTINGS,
                         G_IMPLEMENT_INTERFACE (GIMP_TYPE_CONFIG,
                                                gimp_hue_chroma_config_iface_init))

#define parent_class gimp_hue_chroma_config_parent_class


static void
gimp_hue_chroma_config_class_init (GimpHueChromaConfigClass *klass)
{
  GObjectClass      *object_class   = G_OBJECT_CLASS (klass);
  GimpViewableClass *viewable_class = GIMP_VIEWABLE_CLASS (klass);

  object_class->set_property        = gimp_hue_chroma_config_set_property;
  object_class->get_property        = gimp_hue_chroma_config_get_property;

  viewable_class->default_icon_name = "gimp-tool-hue-chroma";

  GIMP_CONFIG_PROP_ENUM (object_class, PROP_RANGE,
                         "range",
                         _("Range"),
                         _("The affected range"),
                         GIMP_TYPE_HUE_RANGE,
                         GIMP_ALL_HUES, 0);

  GIMP_CONFIG_PROP_DOUBLE (object_class, PROP_HUE,
                           "hue",
                           _("Hue"),
                           _("Hue"),
                           -180.0, 180.0, 0.0, 0);

  GIMP_CONFIG_PROP_DOUBLE (object_class, PROP_CHROMA,
                           "chroma",
                           _("Chroma"),
                           _("Chroma"),
                           -100.0, 100.0, 0.0, 0);

  GIMP_CONFIG_PROP_DOUBLE (object_class, PROP_LIGHTNESS,
                           "lightness",
                           _("Lightness"),
                           _("Lightness"),
                           -100.0, 100.0, 0.0, 0);

  GIMP_CONFIG_PROP_DOUBLE (object_class, PROP_OVERLAP,
                           "overlap",
                           _("Overlap"),
                           _("Overlap"),
                           0.0, 1.0, 0.0, 0);
}

static void
gimp_hue_chroma_config_iface_init (GimpConfigInterface *iface)
{
  iface->serialize   = gimp_hue_chroma_config_serialize;
  iface->deserialize = gimp_hue_chroma_config_deserialize;
  iface->equal       = gimp_hue_chroma_config_equal;
  iface->reset       = gimp_hue_chroma_config_reset;
  iface->copy        = gimp_hue_chroma_config_copy;
}

static void
gimp_hue_chroma_config_init (GimpHueChromaConfig *self)
{
  gimp_config_reset (GIMP_CONFIG (self));
}

static void
gimp_hue_chroma_config_get_property (GObject    *object,
                                         guint       property_id,
                                         GValue     *value,
                                         GParamSpec *pspec)
{
  GimpHueChromaConfig *self = GIMP_HUE_CHROMA_CONFIG (object);

  switch (property_id)
    {
    case PROP_RANGE:
      g_value_set_enum (value, self->range);
      break;

    case PROP_HUE:
      g_value_set_double (value, self->hue[self->range]);
      break;

    case PROP_CHROMA:
      g_value_set_double (value, self->chroma[self->range]);
      break;

    case PROP_LIGHTNESS:
      g_value_set_double (value, self->lightness[self->range]);
      break;

    case PROP_OVERLAP:
      g_value_set_double (value, self->overlap);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_hue_chroma_config_set_property (GObject      *object,
                                         guint         property_id,
                                         const GValue *value,
                                         GParamSpec   *pspec)
{
  GimpHueChromaConfig *self = GIMP_HUE_CHROMA_CONFIG (object);

  switch (property_id)
    {
    case PROP_RANGE:
      self->range = g_value_get_enum (value);
      g_object_notify (object, "hue");
      g_object_notify (object, "chroma");
      g_object_notify (object, "lightness");
      break;

    case PROP_HUE:
      self->hue[self->range] = g_value_get_double (value);
      break;

    case PROP_CHROMA:
      self->chroma[self->range] = g_value_get_double (value);
      break;

    case PROP_LIGHTNESS:
      self->lightness[self->range] = g_value_get_double (value);
      break;

    case PROP_OVERLAP:
      self->overlap = g_value_get_double (value);
      break;

   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static gboolean
gimp_hue_chroma_config_serialize (GimpConfig       *config,
                                      GimpConfigWriter *writer,
                                      gpointer          data)
{
  GimpHueChromaConfig *hs_config = GIMP_HUE_CHROMA_CONFIG (config);
  GimpHueRange             range;
  GimpHueRange             old_range;
  gboolean                 success = TRUE;

  if (! gimp_config_serialize_property_by_name (config, "time", writer))
    return FALSE;

  old_range = hs_config->range;

  for (range = GIMP_ALL_HUES; range <= GIMP_MAGENTA_HUES; range++)
    {
      hs_config->range = range;

      success = (gimp_config_serialize_property_by_name (config, "range",
                                                         writer) &&
                 gimp_config_serialize_property_by_name (config, "hue",
                                                         writer) &&
                 gimp_config_serialize_property_by_name (config, "chroma",
                                                         writer) &&
                 gimp_config_serialize_property_by_name (config, "lightness",
                                                         writer));

      if (! success)
        break;
    }

  if (success)
    success = gimp_config_serialize_property_by_name (config, "overlap",
                                                      writer);

  hs_config->range = old_range;

  return success;
}

static gboolean
gimp_hue_chroma_config_deserialize (GimpConfig *config,
                                        GScanner   *scanner,
                                        gint        nest_level,
                                        gpointer    data)
{
  GimpHueChromaConfig *hs_config = GIMP_HUE_CHROMA_CONFIG (config);
  GimpHueRange             old_range;
  gboolean                 success = TRUE;

  old_range = hs_config->range;

  success = gimp_config_deserialize_properties (config, scanner, nest_level);

  g_object_set (config, "range", old_range, NULL);

  return success;
}

static gboolean
gimp_hue_chroma_config_equal (GimpConfig *a,
                                  GimpConfig *b)
{
  GimpHueChromaConfig *config_a = GIMP_HUE_CHROMA_CONFIG (a);
  GimpHueChromaConfig *config_b = GIMP_HUE_CHROMA_CONFIG (b);
  GimpHueRange             range;

  for (range = GIMP_ALL_HUES; range <= GIMP_MAGENTA_HUES; range++)
    {
      if (config_a->hue[range]        != config_b->hue[range]    ||
          config_a->chroma[range]     != config_b->chroma[range] ||
          config_a->lightness[range]  != config_b->lightness[range])
        return FALSE;
    }

  /* don't compare "range" */

  if (config_a->overlap != config_b->overlap)
    return FALSE;

  return TRUE;
}

static void
gimp_hue_chroma_config_reset (GimpConfig *config)
{
  GimpHueChromaConfig *hs_config = GIMP_HUE_CHROMA_CONFIG (config);
  GimpHueRange             range;

  for (range = GIMP_ALL_HUES; range <= GIMP_MAGENTA_HUES; range++)
    {
      hs_config->range = range;
      gimp_hue_chroma_config_reset_range (hs_config);
    }

  gimp_config_reset_property (G_OBJECT (config), "range");
  gimp_config_reset_property (G_OBJECT (config), "overlap");
}

static gboolean
gimp_hue_chroma_config_copy (GimpConfig   *src,
                                 GimpConfig   *dest,
                                 GParamFlags   flags)
{
  GimpHueChromaConfig *src_config  = GIMP_HUE_CHROMA_CONFIG (src);
  GimpHueChromaConfig *dest_config = GIMP_HUE_CHROMA_CONFIG (dest);
  GimpHueRange             range;

  for (range = GIMP_ALL_HUES; range <= GIMP_MAGENTA_HUES; range++)
    {
      dest_config->hue[range]        = src_config->hue[range];
      dest_config->chroma[range]     = src_config->chroma[range];
      dest_config->lightness[range]  = src_config->lightness[range];
    }

  g_object_notify (G_OBJECT (dest), "hue");
  g_object_notify (G_OBJECT (dest), "chroma");
  g_object_notify (G_OBJECT (dest), "lightness");

  dest_config->range   = src_config->range;
  dest_config->overlap = src_config->overlap;

  g_object_notify (G_OBJECT (dest), "range");
  g_object_notify (G_OBJECT (dest), "overlap");

  return TRUE;
}


/*  public functions  */

void
gimp_hue_chroma_config_reset_range (GimpHueChromaConfig *config)
{
  g_return_if_fail (GIMP_IS_HUE_CHROMA_CONFIG (config));

  g_object_freeze_notify (G_OBJECT (config));

  gimp_config_reset_property (G_OBJECT (config), "hue");
  gimp_config_reset_property (G_OBJECT (config), "chroma");
  gimp_config_reset_property (G_OBJECT (config), "lightness");

  g_object_thaw_notify (G_OBJECT (config));
}
