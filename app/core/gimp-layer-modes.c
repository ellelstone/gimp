/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimp-layer-modes.c
 * Copyright (C) 2017 Michael Natterer <mitch@gimp.org>
 *                    Øyvind Kolås <pippin@gimp.org>
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

#include "config.h"

#include <glib-object.h>

#include "core-types.h"

#include "gimp-layer-modes.h"


gboolean
gimp_layer_mode_is_legacy (GimpLayerMode  mode)
{
  return FALSE;
}

gboolean
gimp_layer_mode_is_linear (GimpLayerMode  mode)
{
  switch (mode)
    {
    case GIMP_LAYER_MODE_NORMAL:
      return FALSE;

    case GIMP_LAYER_MODE_DISSOLVE:
      return FALSE;

    case GIMP_LAYER_MODE_BEHIND:
      return FALSE;

    case GIMP_LAYER_MODE_COLOR_ERASE:
    case GIMP_LAYER_MODE_OVERLAY:
      return FALSE;

    case GIMP_LAYER_MODE_LCH_HUE:
    case GIMP_LAYER_MODE_LCH_CHROMA:
    case GIMP_LAYER_MODE_LCH_COLOR:
    case GIMP_LAYER_MODE_LCH_LIGHTNESS:
    case GIMP_LAYER_MODE_LUMINANCE:
      return FALSE;

    case GIMP_LAYER_MODE_MULTIPLY:
    case GIMP_LAYER_MODE_SCREEN:
    case GIMP_LAYER_MODE_DIFFERENCE:
    case GIMP_LAYER_MODE_ADDITION:
    case GIMP_LAYER_MODE_SUBTRACT:
    case GIMP_LAYER_MODE_DARKEN_ONLY:
    case GIMP_LAYER_MODE_LIGHTEN_ONLY:
    case GIMP_LAYER_MODE_DIVIDE:
    case GIMP_LAYER_MODE_DODGE:
    case GIMP_LAYER_MODE_BURN:
    case GIMP_LAYER_MODE_HARDLIGHT:
    case GIMP_LAYER_MODE_SOFTLIGHT:
    case GIMP_LAYER_MODE_GRAIN_EXTRACT:
    case GIMP_LAYER_MODE_GRAIN_MERGE:
      return FALSE;

    case GIMP_LAYER_MODE_ERASE:
      return FALSE;

    case GIMP_LAYER_MODE_REPLACE:
      return FALSE;

    case GIMP_LAYER_MODE_ANTI_ERASE:
      return FALSE;
  }

  return FALSE;
}

GimpLayerColorSpace
gimp_layer_mode_get_blend_space (GimpLayerMode  mode)
{
  switch (mode)
    {
    case GIMP_LAYER_MODE_NORMAL:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_DISSOLVE:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_BEHIND:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_COLOR_ERASE:
    case GIMP_LAYER_MODE_OVERLAY:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_LCH_HUE:
    case GIMP_LAYER_MODE_LCH_CHROMA:
    case GIMP_LAYER_MODE_LCH_COLOR:
    case GIMP_LAYER_MODE_LCH_LIGHTNESS:
      return GIMP_LAYER_COLOR_SPACE_LAB;

    case GIMP_LAYER_MODE_LUMINANCE:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_MULTIPLY:
    case GIMP_LAYER_MODE_SCREEN:
    case GIMP_LAYER_MODE_DIFFERENCE:
    case GIMP_LAYER_MODE_ADDITION:
    case GIMP_LAYER_MODE_SUBTRACT:
    case GIMP_LAYER_MODE_DARKEN_ONLY:
    case GIMP_LAYER_MODE_LIGHTEN_ONLY:
    case GIMP_LAYER_MODE_DIVIDE:
    case GIMP_LAYER_MODE_DODGE:
    case GIMP_LAYER_MODE_BURN:
    case GIMP_LAYER_MODE_HARDLIGHT:
    case GIMP_LAYER_MODE_SOFTLIGHT:
    case GIMP_LAYER_MODE_GRAIN_EXTRACT:
    case GIMP_LAYER_MODE_GRAIN_MERGE:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_ERASE:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_REPLACE:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;

    case GIMP_LAYER_MODE_ANTI_ERASE:
      return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;
  }

  return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;
}

GimpLayerColorSpace
gimp_layer_mode_get_composite_space (GimpLayerMode  mode)
{
  return GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL;
}

GimpLayerCompositeMode
gimp_layer_mode_get_composite_mode (GimpLayerMode  mode)
{
  return GIMP_LAYER_COMPOSITE_SRC_OVER;
}

const gchar *
gimp_layer_mode_get_operation (GimpLayerMode  mode)
{
  switch (mode)
    {
    case GIMP_LAYER_MODE_NORMAL:
      return "gimp:normal";

    case GIMP_LAYER_MODE_DISSOLVE:
      return "gimp:dissolve";

    case GIMP_LAYER_MODE_BEHIND:
      return "gimp:behind";

    case GIMP_LAYER_MODE_MULTIPLY:
      return "gimp:multiply";

    case GIMP_LAYER_MODE_SCREEN:
      return "gimp:screen";

    case GIMP_LAYER_MODE_OVERLAY:
      return "gimp:overlay";

    case GIMP_LAYER_MODE_DIFFERENCE:
      return "gimp:difference";

    case GIMP_LAYER_MODE_ADDITION:
      return "gimp:addition";

    case GIMP_LAYER_MODE_SUBTRACT:
      return "gimp:subtract";

    case GIMP_LAYER_MODE_DARKEN_ONLY:
      return "gimp:darken-only";

    case GIMP_LAYER_MODE_LIGHTEN_ONLY:
      return "gimp:lighten-only";

    case GIMP_LAYER_MODE_DIVIDE:
      return "gimp:divide";

    case GIMP_LAYER_MODE_DODGE:
      return "gimp:dodge";

    case GIMP_LAYER_MODE_BURN:
      return "gimp:burn";

    case GIMP_LAYER_MODE_HARDLIGHT:
      return "gimp:hardlight";

    case GIMP_LAYER_MODE_SOFTLIGHT:
      return "gimp:softlight";

    case GIMP_LAYER_MODE_GRAIN_EXTRACT:
      return "gimp:grain-extract";

    case GIMP_LAYER_MODE_GRAIN_MERGE:
      return "gimp:grain-merge";

    case GIMP_LAYER_MODE_COLOR_ERASE:
      return "gimp:color-erase";

    case GIMP_LAYER_MODE_LCH_HUE:
      return "gimp:lch-hue";

    case GIMP_LAYER_MODE_LCH_CHROMA:
      return "gimp:lch-chroma";

    case GIMP_LAYER_MODE_LCH_COLOR:
      return "gimp:lch-color";

    case GIMP_LAYER_MODE_LCH_LIGHTNESS:
      return "gimp:lch-lightness";

    case GIMP_LAYER_MODE_LUMINANCE:
      return "gimp:luminance";

    case GIMP_LAYER_MODE_ERASE:
      return "gimp:erase";

    case GIMP_LAYER_MODE_REPLACE:
      return "gimp:replace";

    case GIMP_LAYER_MODE_ANTI_ERASE:
      return "gimp:anti-erase";
    }

  return NULL;
}
