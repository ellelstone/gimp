/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * operations-enums.h
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

#ifndef __OPERATIONS_ENUMS_H__
#define __OPERATIONS_ENUMS_H__


#define GIMP_TYPE_LAYER_COLOR_SPACE (gimp_layer_color_space_get_type ())

GType gimp_layer_color_space_get_type (void) G_GNUC_CONST;

typedef enum
{
  GIMP_LAYER_COLOR_SPACE_AUTO,           /*< desc="Auto"             >*/
  GIMP_LAYER_COLOR_SPACE_RGB_PERCEPTUAL, /*< desc="RGB (perceptual)" >*/
  GIMP_LAYER_COLOR_SPACE_LAB,            /*< desc="LAB", pdb-skip    >*/
} GimpLayerColorSpace;


#define GIMP_TYPE_LAYER_COMPOSITE_MODE (gimp_layer_composite_mode_get_type ())

GType gimp_layer_composite_mode_get_type (void) G_GNUC_CONST;

typedef enum
{
  GIMP_LAYER_COMPOSITE_AUTO,      /*< desc="Auto"             >*/
  GIMP_LAYER_COMPOSITE_SRC_OVER,  /*< desc="Source over"      >*/
  GIMP_LAYER_COMPOSITE_SRC_ATOP,  /*< desc="Source atop"      >*/
  GIMP_LAYER_COMPOSITE_DST_ATOP,  /*< desc="Destination atop" >*/
  GIMP_LAYER_COMPOSITE_SRC_IN     /*< desc="Source in"        >*/
} GimpLayerCompositeMode;


#define GIMP_TYPE_LAYER_MODE (gimp_layer_mode_get_type ())

GType gimp_layer_mode_get_type (void) G_GNUC_CONST;

typedef enum
{
  /*  Modes that exist since ancient times  */
  GIMP_LAYER_MODE_DISSOLVE,              /*< desc="Dissolve"                 >*/

  /*  Since 2.10 (XCF version 9) */
  GIMP_LAYER_MODE_OVERLAY,               /*< desc="Overlay"                  >*/
  GIMP_LAYER_MODE_LCH_HUE,               /*< desc="Hue (LCH)"                >*/
  GIMP_LAYER_MODE_LCH_CHROMA,            /*< desc="Chroma (LCH)"             >*/
  GIMP_LAYER_MODE_LCH_COLOR,             /*< desc="Color (LCH)"              >*/
  GIMP_LAYER_MODE_LCH_LIGHTNESS,         /*< desc="Lightness (LCH)"          >*/

  /*  Since 2.10 (XCF version 10)  */
  GIMP_LAYER_MODE_NORMAL,                /*< desc="Normal"                   >*/
  GIMP_LAYER_MODE_BEHIND,                /*< desc="Behind"                   >*/
  GIMP_LAYER_MODE_MULTIPLY,              /*< desc="Multiply"                 >*/
  GIMP_LAYER_MODE_SCREEN,                /*< desc="Screen"                   >*/
  GIMP_LAYER_MODE_DIFFERENCE,            /*< desc="Difference"               >*/
  GIMP_LAYER_MODE_ADDITION,              /*< desc="Addition"                 >*/
  GIMP_LAYER_MODE_SUBTRACT,              /*< desc="Subtract"                 >*/
  GIMP_LAYER_MODE_DARKEN_ONLY,           /*< desc="Darken only"              >*/
  GIMP_LAYER_MODE_LIGHTEN_ONLY,          /*< desc="Lighten only"             >*/
  GIMP_LAYER_MODE_HSV_HUE,               /*< desc="Hue (HSV)"                >*/
  GIMP_LAYER_MODE_HSV_SATURATION,        /*< desc="Saturation (HSV)"         >*/
  GIMP_LAYER_MODE_HSL_COLOR,             /*< desc="Color (HSL)"              >*/
  GIMP_LAYER_MODE_HSV_VALUE,             /*< desc="Value (HSV)"              >*/
  GIMP_LAYER_MODE_DIVIDE,                /*< desc="Divide"                   >*/
  GIMP_LAYER_MODE_DODGE,                 /*< desc="Dodge"                    >*/
  GIMP_LAYER_MODE_BURN,                  /*< desc="Burn"                     >*/
  GIMP_LAYER_MODE_HARDLIGHT,             /*< desc="Hard light"               >*/
  GIMP_LAYER_MODE_SOFTLIGHT,             /*< desc="Soft light"               >*/
  GIMP_LAYER_MODE_GRAIN_EXTRACT,         /*< desc="Grain extract"            >*/
  GIMP_LAYER_MODE_GRAIN_MERGE,           /*< desc="Grain merge"              >*/
  GIMP_LAYER_MODE_VIVID_LIGHT,           /*< desc="Vivid light"              >*/
  GIMP_LAYER_MODE_PIN_LIGHT,             /*< desc="Pin light"                >*/
  GIMP_LAYER_MODE_LINEAR_LIGHT,          /*< desc="Linear light"             >*/
  GIMP_LAYER_MODE_HARD_MIX,              /*< desc="Hard mix"                 >*/
  GIMP_LAYER_MODE_EXCLUSION,             /*< desc="Exclusion"                >*/
  GIMP_LAYER_MODE_LINEAR_BURN,           /*< desc="Linear burn"              >*/
  GIMP_LAYER_MODE_LUMA_DARKEN_ONLY,   /*< desc="Luma/Luminance darken only"  >*/
  GIMP_LAYER_MODE_LUMA_LIGHTEN_ONLY,  /*< desc="Luma/Luminance lighten only" >*/
  GIMP_LAYER_MODE_LUMINANCE,             /*< desc="Luminance"                >*/
  GIMP_LAYER_MODE_COLOR_ERASE,           /*< desc="Color erase"              >*/
  GIMP_LAYER_MODE_ERASE,                 /*< desc="Erase"                    >*/
  GIMP_LAYER_MODE_MERGE,                 /*< desc="Merge"                    >*/
  GIMP_LAYER_MODE_SPLIT,                 /*< desc="Split"                    >*/
  GIMP_LAYER_MODE_MONO_MIX,              /*< desc="Mono mix"                 >*/
  GIMP_LAYER_MODE_PASS_THROUGH,          /*< desc="Pass through"             >*/

  /*  Internal modes, not available to the PDB, must be kept at the end  */
  GIMP_LAYER_MODE_REPLACE,               /*< pdb-skip, desc="Replace"        >*/
  GIMP_LAYER_MODE_ANTI_ERASE,            /*< pdb-skip, desc="Anti erase"     >*/

  /*  Layer mode menu separator  */
  GIMP_LAYER_MODE_SEPARATOR = -1         /*< pdb-skip, skip                  >*/
} GimpLayerMode;


#define GIMP_TYPE_LAYER_MODE_GROUP (gimp_layer_mode_group_get_type ())

GType gimp_layer_mode_group_get_type (void) G_GNUC_CONST;

typedef enum  /*< pdb-skip >*/
{
  GIMP_LAYER_MODE_GROUP_DEFAULT
} GimpLayerModeGroup;


#define GIMP_TYPE_LAYER_MODE_CONTEXT (gimp_layer_mode_context_get_type ())

GType gimp_layer_mode_context_get_type (void) G_GNUC_CONST;

typedef enum  /*< pdb-skip >*/
{
  GIMP_LAYER_MODE_CONTEXT_LAYER = 1 << 0,
  GIMP_LAYER_MODE_CONTEXT_GROUP = 1 << 1,
  GIMP_LAYER_MODE_CONTEXT_PAINT = 1 << 2,
  GIMP_LAYER_MODE_CONTEXT_FADE  = 1 << 3,

  GIMP_LAYER_MODE_CONTEXT_ALL = (GIMP_LAYER_MODE_CONTEXT_LAYER |
                                 GIMP_LAYER_MODE_CONTEXT_GROUP |
                                 GIMP_LAYER_MODE_CONTEXT_PAINT |
                                 GIMP_LAYER_MODE_CONTEXT_FADE)
} GimpLayerModeContext;


/*
 * non-registered enums; register them if needed
 */

typedef enum  /*< pdb-skip, skip >*/
{
  GIMP_LAYER_COMPOSITE_REGION_INTERSECTION = 0,
  GIMP_LAYER_COMPOSITE_REGION_DESTINATION  = 1 << 0,
  GIMP_LAYER_COMPOSITE_REGION_SOURCE       = 1 << 1,
  GIMP_LAYER_COMPOSITE_REGION_UNION        = (GIMP_LAYER_COMPOSITE_REGION_DESTINATION |
                                              GIMP_LAYER_COMPOSITE_REGION_SOURCE),
} GimpLayerCompositeRegion;

typedef enum  /*< pdb-skip, skip >*/
{
  GIMP_LAYER_MODE_FLAG_LEGACY                    =  1 << 0,
  GIMP_LAYER_MODE_FLAG_BLEND_SPACE_IMMUTABLE     =  1 << 1,
  GIMP_LAYER_MODE_FLAG_COMPOSITE_SPACE_IMMUTABLE =  1 << 2,
  GIMP_LAYER_MODE_FLAG_COMPOSITE_MODE_IMMUTABLE  =  1 << 3,
  GIMP_LAYER_MODE_FLAG_SUBTRACTIVE               =  1 << 4
} GimpLayerModeFlags;


#endif /* __OPERATIONS_ENUMS_H__ */
