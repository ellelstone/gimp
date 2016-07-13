/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimphuechromaconfig.h
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

#ifndef __GIMP_HUE_CHROMA_CONFIG_H__
#define __GIMP_HUE_CHROMA_CONFIG_H__


#include "core/gimpsettings.h"


#define GIMP_TYPE_HUE_CHROMA_CONFIG            (gimp_hue_chroma_config_get_type ())
#define GIMP_HUE_CHROMA_CONFIG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_HUE_CHROMA_CONFIG, GimpHueChromaConfig))
#define GIMP_HUE_CHROMA_CONFIG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GIMP_TYPE_HUE_CHROMA_CONFIG, GimpHueChromaConfigClass))
#define GIMP_IS_HUE_CHROMA_CONFIG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_HUE_CHROMA_CONFIG))
#define GIMP_IS_HUE_CHROMA_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GIMP_TYPE_HUE_CHROMA_CONFIG))
#define GIMP_HUE_CHROMA_CONFIG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GIMP_TYPE_HUE_CHROMA_CONFIG, GimpHueChromaConfigClass))


typedef struct _GimpHueChromaConfigClass GimpHueChromaConfigClass;

struct _GimpHueChromaConfig
{
  GimpSettings  parent_instance;

  GimpHueRange  range;

  gdouble       hue[7];
  gdouble       chroma[7];
  gdouble       lightness[7];

  gdouble       overlap;
};

struct _GimpHueChromaConfigClass
{
  GimpSettingsClass  parent_class;
};


GType   gimp_hue_chroma_config_get_type    (void) G_GNUC_CONST;

void    gimp_hue_chroma_config_reset_range (GimpHueChromaConfig *config);


#endif /* __GIMP_HUE_CHROMA_CONFIG_H__ */
