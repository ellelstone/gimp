/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpoperationhuechroma.c
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
#include <gegl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "libgimpcolor/gimpcolor.h"
#include "libgimpmath/gimpmath.h"

#include "operations-types.h"

#include "gimphuesaturationconfig.h"
#include "gimpoperationhuesaturation.h"


static gboolean gimp_operation_hue_chroma_process (GeglOperation       *operation,
                                                       void                *in_buf,
                                                       void                *out_buf,
                                                       glong                samples,
                                                       const GeglRectangle *roi,
                                                       gint                 level);


G_DEFINE_TYPE (GimpOperationHueChroma, gimp_operation_hue_chroma,
               GIMP_TYPE_OPERATION_POINT_FILTER)

#define parent_class gimp_operation_hue_chroma_parent_class


static void
gimp_operation_hue_chroma_class_init (GimpOperationHueChromaClass *klass)
{
  GObjectClass                  *object_class    = G_OBJECT_CLASS (klass);
  GeglOperationClass            *operation_class = GEGL_OPERATION_CLASS (klass);
  GeglOperationPointFilterClass *point_class     = GEGL_OPERATION_POINT_FILTER_CLASS (klass);

  object_class->set_property   = gimp_operation_point_filter_set_property;
  object_class->get_property   = gimp_operation_point_filter_get_property;

  gegl_operation_class_set_keys (operation_class,
                                 "name",        "gimp:hue-chroma",
                                 "categories",  "color",
                                 "description", "GIMP Hue-Saturation operation",
                                 NULL);

  point_class->process = gimp_operation_hue_chroma_process;

  g_object_class_install_property (object_class,
                                   GIMP_OPERATION_POINT_FILTER_PROP_CONFIG,
                                   g_param_spec_object ("config",
                                                        "Config",
                                                        "The config object",
                                                        GIMP_TYPE_HUE_CHROMA_CONFIG,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT));
}

static void
gimp_operation_hue_chroma_init (GimpOperationHueChroma *self)
{
}

static inline gdouble
map_lightness (GimpHueChromaConfig *config,
               GimpHueRange             range,
               gdouble                  value)
{
  value += config->lightness[GIMP_ALL_HUES];

    return value;
}

static inline gdouble
map_chroma (GimpHueChromaConfig *config,
            GimpHueRange         range,
            gdouble              value)
{
  value += config->chroma[GIMP_ALL_HUES];

  return CLAMP (value, 0.0, 200.0);
}

static inline gdouble
map_hue (GimpHueChromaConfig *config,
         GimpHueRange             range,
         gdouble                  value)
{
  value += config->hue[GIMP_ALL_HUES];

    return value;
}

static gboolean
gimp_operation_hue_chroma_process (GeglOperation       *operation,
                                       void                *in_buf,
                                       void                *out_buf,
                                       glong                samples,
                                       const GeglRectangle *roi,
                                       gint                 level)
{
  GimpOperationPointFilter *point  = GIMP_OPERATION_POINT_FILTER (operation);
  GimpHueChromaConfig  *config = GIMP_HUE_CHROMA_CONFIG (point->config);
  gfloat                   *src    = in_buf;
  gfloat                   *dest   = out_buf;

  if (! config)
    return FALSE;

  while (samples--)
    {
      GimpRGB  rgb;
      GimpLch  lch;

      rgb.r = src[RED];
      rgb.g = src[GREEN];
      rgb.b = src[BLUE];
      rgb.a = src[ALPHA];

      babl_process (babl_fish ("RGBA double", "CIE LCH(ab) alpha double"), &rgb, &lch, 1);

          lch.l = map_lightness  (config, 0, lch.l);
          lch.c = map_chroma (config, 0, lch.c);
          lch.h = map_hue        (config, 0, lch.h);


      babl_process (babl_fish ("CIE LCH(ab) alpha double", "RGBA double"), &lch, &rgb, 1);

      dest[RED]   = rgb.r;
      dest[GREEN] = rgb.g;
      dest[BLUE]  = rgb.b;
      dest[ALPHA] = rgb.a;

      src  += 4;
      dest += 4;
    }

  return TRUE;
}


/*  public functions  */

void
gimp_operation_hue_chroma_map (GimpHueChromaConfig *config,
                               const GimpRGB           *color,
                               GimpHueRange             range,
                               GimpRGB                 *result)
{
  GimpLch lch;

  g_return_if_fail (GIMP_IS_HUE_CHROMA_CONFIG (config));
  g_return_if_fail (color != NULL);
  g_return_if_fail (result != NULL);

  babl_process (babl_fish ("RGBA double", "CIE LCH(ab) alpha double"), color, &lch, 1);

  lch.l = map_lightness  (config, range, lch.l);
  lch.c = map_chroma (config, range, lch.c);
  lch.h = map_hue        (config, range, lch.h);

  babl_process (babl_fish ("CIE LCH(ab) alpha double", "RGBA double"), &lch, result, 1);
}
