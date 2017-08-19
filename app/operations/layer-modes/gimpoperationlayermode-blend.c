/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpoperationlayermode-blend.c
 * Copyright (C) 2017 Michael Natterer <mitch@gimp.org>
 *               2017 Øyvind Kolås <pippin@gimp.org>
 *               2017 Ell
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

#include <gegl-plugin.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "libgimpcolor/gimpcolor.h"
#include "libgimpbase/gimpbase.h"
#include "libgimpmath/gimpmath.h"

#include "../operations-types.h"

#include "gimpoperationlayermode-blend.h"


/*  non-subtractive blending functions.  these functions must set comp[ALPHA]
 *  to the same value as layer[ALPHA].  when in[ALPHA] or layer[ALPHA] are
 *  zero, the value of comp[RED..BLUE] is unconstrained (in particular, it may
 *  be NaN).
 */


void /* aka linear_dodge */
gimp_operation_layer_mode_blend_addition (const gfloat *in,
                                          const gfloat *layer,
                                          gfloat       *comp,
                                          gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = in[c] + layer[c];
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_burn (const gfloat *in,
                                      const gfloat *layer,
                                      gfloat       *comp,
                                      gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val = 1.0f - (1.0f - in[c]) / layer[c];

              /* The CLAMP macro is deliberately inlined and written
               * to map comp == NAN (0 / 0) -> 1
               */
              comp[c] = val < 0 ? 0.0f : val < 1.0f ? val : 1.0f;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_darken_only (const gfloat *in,
                                             const gfloat *layer,
                                             gfloat       *comp,
                                             gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = MIN (in[c], layer[c]);
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_difference (const gfloat *in,
                                            const gfloat *layer,
                                            gfloat       *comp,
                                            gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              comp[c] = in[c] - layer[c];

              if (comp[c] < 0)
                comp[c] = -comp[c];
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_divide (const gfloat *in,
                                        const gfloat *layer,
                                        gfloat       *comp,
                                        gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val = in[c] / layer[c];

              /* make infinities(or NaN) correspond to a high number,
               * to get more predictable math, ideally higher than 5.0
               * but it seems like some babl conversions might be
               * acting up then
               */
              if (!(val > -42949672.0f && val < 5.0f))
                val = 5.0f;

              comp[c] = val;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_dodge (const gfloat *in,
                                       const gfloat *layer,
                                       gfloat       *comp,
                                       gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val = in[c] / (1.0f - layer[c]);

              val = MIN (val, 1.0f);

              comp[c] = val;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_exclusion (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              comp[c] = 0.5f - 2.0f * (in[c] - 0.5f) * (layer[c] - 0.5f);
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_grain_extract (const gfloat *in,
                                               const gfloat *layer,
                                               gfloat       *comp,
                                               gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = in[c] - layer[c] + 0.5f;
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_grain_merge (const gfloat *in,
                                             const gfloat *layer,
                                             gfloat       *comp,
                                             gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = in[c] + layer[c] - 0.5f;
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_hard_mix (const gfloat *in,
                                          const gfloat *layer,
                                          gfloat       *comp,
                                          gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              comp[c] = in[c] + layer[c] < 1.0f ? 0.0f : 1.0f;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_hardlight (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val;

              if (layer[c] > 0.5f)
                {
                  val = (1.0f - in[c]) * (1.0f - (layer[c] - 0.5f) * 2.0f);
                  val = MIN (1.0f - val, 1.0f);
                }
              else
                {
                  val = in[c] * (layer[c] * 2.0f);
                  val = MIN (val, 1.0f);
                }

              comp[c] = val;
            }
        }
      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_hsl_color (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gfloat dest_min, dest_max, dest_l;
          gfloat src_min,  src_max,  src_l;

          dest_min = MIN (in[0],  in[1]);
          dest_min = MIN (dest_min, in[2]);
          dest_max = MAX (in[0],  in[1]);
          dest_max = MAX (dest_max, in[2]);
          dest_l   = (dest_min + dest_max) / 2.0f;

          src_min  = MIN (layer[0],  layer[1]);
          src_min  = MIN (src_min, layer[2]);
          src_max  = MAX (layer[0],  layer[1]);
          src_max  = MAX (src_max, layer[2]);
          src_l    = (src_min + src_max) / 2.0f;

          if (src_l != 0.0f && src_l != 1.0f)
            {
              gboolean dest_high;
              gboolean src_high;
              gfloat   ratio;
              gfloat   offset;
              gint     c;

              dest_high = dest_l > 0.5f;
              src_high  = src_l  > 0.5f;

              dest_l = MIN (dest_l, 1.0f - dest_l);
              src_l  = MIN (src_l,  1.0f - src_l);

              ratio                  = dest_l / src_l;

              offset                 = 0.0f;
              if (dest_high) offset += 1.0f - 2.0f * dest_l;
              if (src_high)  offset += 2.0f * dest_l - ratio;

              for (c = 0; c < 3; c++)
                comp[c] = layer[c] * ratio + offset;
            }
          else
            {
              comp[RED]   = dest_l;
              comp[GREEN] = dest_l;
              comp[BLUE]  = dest_l;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_hsv_hue (const gfloat *in,
                                         const gfloat *layer,
                                         gfloat       *comp,
                                         gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gfloat src_min,  src_max,  src_delta;
          gfloat dest_min, dest_max, dest_delta, dest_s;

          src_min   = MIN (layer[0], layer[1]);
          src_min   = MIN (src_min, layer[2]);
          src_max   = MAX (layer[0], layer[1]);
          src_max   = MAX (src_max, layer[2]);
          src_delta = src_max - src_min;

          if (src_delta != 0.0f)
            {
              gfloat ratio;
              gfloat offset;
              gint   c;

              dest_min   = MIN (in[0], in[1]);
              dest_min   = MIN (dest_min, in[2]);
              dest_max   = MAX (in[0], in[1]);
              dest_max   = MAX (dest_max, in[2]);
              dest_delta = dest_max - dest_min;
              dest_s     = dest_max ? dest_delta / dest_max : 0.0f;

              ratio  = dest_s * dest_max / src_delta;
              offset = dest_max - src_max * ratio;

              for (c = 0; c < 3; c++)
                comp[c] = layer[c] * ratio + offset;
            }
          else
            {
              gint c;

              for (c = 0; c < 3; c++)
                comp[c] = in[c];
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_hsv_saturation (const gfloat *in,
                                                const gfloat *layer,
                                                gfloat       *comp,
                                                gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gfloat src_min,  src_max,  src_delta, src_s;
          gfloat dest_min, dest_max, dest_delta;

          dest_min   = MIN (in[0], in[1]);
          dest_min   = MIN (dest_min, in[2]);
          dest_max   = MAX (in[0], in[1]);
          dest_max   = MAX (dest_max, in[2]);
          dest_delta = dest_max - dest_min;

          if (dest_delta != 0.0f)
            {
              gfloat ratio;
              gfloat offset;
              gint   c;

              src_min   = MIN (layer[0], layer[1]);
              src_min   = MIN (src_min, layer[2]);
              src_max   = MAX (layer[0], layer[1]);
              src_max   = MAX (src_max, layer[2]);
              src_delta = src_max - src_min;
              src_s     = src_max ? src_delta / src_max : 0.0f;

              ratio  = src_s * dest_max / dest_delta;
              offset = (1.0f - ratio) * dest_max;

              for (c = 0; c < 3; c++)
                comp[c] = in[c] * ratio + offset;
            }
          else
            {
              comp[RED]   = dest_max;
              comp[GREEN] = dest_max;
              comp[BLUE]  = dest_max;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_hsv_value (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gfloat dest_v;
          gfloat src_v;

          dest_v = MAX (in[0], in[1]);
          dest_v = MAX (dest_v, in[2]);

          src_v  = MAX (layer[0], layer[1]);
          src_v  = MAX (src_v, layer[2]);

          if (dest_v != 0.0f)
            {
              gfloat ratio = src_v / dest_v;
              gint   c;

              for (c = 0; c < 3; c++)
                comp[c] = in[c] * ratio;
            }
          else
            {
              comp[RED]   = src_v;
              comp[GREEN] = src_v;
              comp[BLUE]  = src_v;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_lch_chroma (const gfloat *in,
                                            const gfloat *layer,
                                            gfloat       *comp,
                                            gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gfloat A1 = in[1];
          gfloat B1 = in[2];
          gfloat c1 = hypotf (A1, B1);

          if (c1 != 0.0f)
            {
              gfloat A2 = layer[1];
              gfloat B2 = layer[2];
              gfloat c2 = hypotf (A2, B2);
              gfloat A  = c2 * A1 / c1;
              gfloat B  = c2 * B1 / c1;

              comp[0] = in[0];
              comp[1] = A;
              comp[2] = B;
            }
          else
            {
              comp[0] = in[0];
              comp[1] = in[1];
              comp[2] = in[2];
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_lch_color (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          comp[0] = in[0];
          comp[1] = layer[1];
          comp[2] = layer[2];
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
  }
}

void
gimp_operation_layer_mode_blend_lch_hue (const gfloat *in,
                                         const gfloat *layer,
                                         gfloat       *comp,
                                         gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gfloat A2 = layer[1];
          gfloat B2 = layer[2];
          gfloat c2 = hypotf (A2, B2);

          if (c2 > 0.1f)
            {
              gfloat A1 = in[1];
              gfloat B1 = in[2];
              gfloat c1 = hypotf (A1, B1);
              gfloat A  = c1 * A2 / c2;
              gfloat B  = c1 * B2 / c2;

              comp[0] = in[0];
              comp[1] = A;
              comp[2] = B;
            }
          else
            {
              comp[0] = in[0];
              comp[1] = in[1];
              comp[2] = in[2];
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_lch_lightness (const gfloat *in,
                                               const gfloat *layer,
                                               gfloat       *comp,
                                               gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          comp[0] = layer[0];
          comp[1] = in[1];
          comp[2] = in[2];
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_lighten_only (const gfloat *in,
                                              const gfloat *layer,
                                              gfloat       *comp,
                                              gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = MAX (in[c], layer[c]);
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_linear_burn (const gfloat *in,
                                             const gfloat *layer,
                                             gfloat       *comp,
                                             gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = in[c] + layer[c] - 1.0f;
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

/* added according to:
    http://www.deepskycolors.com/archivo/2010/04/21/formulas-for-Photoshop-blending-modes.html */
void
gimp_operation_layer_mode_blend_linear_light (const gfloat *in,
                                              const gfloat *layer,
                                              gfloat       *comp,
                                              gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val;

              if (layer[c] <= 0.5f)
                {
                  val = in[c] + 2.0 * layer[c] - 1.0f;
                }
              else
                {
                  val = in[c] + 2.0 * (layer[c] - 0.5f);
                }
              comp[c] = val;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_luma_darken_only (const gfloat *in,
                                                  const gfloat *layer,
                                                  gfloat       *comp,
                                                  gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;
          gfloat dest_luminance =
             GIMP_RGB_LUMINANCE(in[0], in[1], in[2]);
          gfloat src_luminance =
             GIMP_RGB_LUMINANCE(layer[0], layer[1], layer[2]);

          if (dest_luminance <= src_luminance)
            for (c = 0; c < 3; c++)
              comp[c] = in[c];
          else
            for (c = 0; c < 3; c++)
              comp[c] = layer[c];
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_luma_lighten_only (const gfloat *in,
                                                   const gfloat *layer,
                                                   gfloat       *comp,
                                                   gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;
          gfloat dest_luminance =
             GIMP_RGB_LUMINANCE(in[0], in[1], in[2]);
          gfloat src_luminance =
             GIMP_RGB_LUMINANCE(layer[0], layer[1], layer[2]);

          if (dest_luminance >= src_luminance)
            for (c = 0; c < 3; c++)
              comp[c] = in[c];
          else
            for (c = 0; c < 3; c++)
              comp[c] = layer[c];
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_luminance (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  gfloat layer_Y[samples], *layer_Y_p;
  gfloat in_Y[samples], *in_Y_p;

  babl_process (babl_fish ("RGBA gfloat", "Y gfloat"), layer, layer_Y, samples);
  babl_process (babl_fish ("RGBA gfloat", "Y gfloat"), in, in_Y, samples);

  layer_Y_p = &layer_Y[0];
  in_Y_p = &in_Y[0];

  while (samples--)
    {
      if (layer[ALPHA] != 0.0f && in[ALPHA] != 0.0f)
        {
          gfloat ratio = layer_Y_p[0] / MAX(in_Y_p[0], 0.0000000000000000001);
          gint c;
          for (c = 0; c < 3; c ++)
            comp[c] = in[c] * ratio;
        }

      comp[ALPHA] = layer[ALPHA];

      comp      += 4;
      in        += 4;
      layer     += 4;
      in_Y_p    ++;
      layer_Y_p ++;
    }
}

void
gimp_operation_layer_mode_blend_mono_mix (const gfloat *in,
                                          const gfloat *layer,
                                          gfloat       *comp,
                                          gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gfloat value = 0.0f;
          gint   c;

          for (c = 0; c < 3; c++)
            {
              value += in[c] * layer[c];
            }

          comp[RED] = comp[GREEN] = comp[BLUE] = value;
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}


void
gimp_operation_layer_mode_blend_multiply (const gfloat *in,
                                          const gfloat *layer,
                                          gfloat       *comp,
                                          gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = in[c] * layer[c];
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_overlay (const gfloat *in,
                                         const gfloat *layer,
                                         gfloat       *comp,
                                         gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val;

              if (in[c] < 0.5f)
                {
                  val = 2.0f * in[c] * layer[c];
                }
              else
                {
                  val = 1.0f - 2.0f * (1.0f - layer[c]) * (1.0f - in[c]);
                }

              comp[c] = val;
            }
        }
      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

/* added according to:
    http://www.deepskycolors.com/archivo/2010/04/21/formulas-for-Photoshop-blending-modes.html */
void
gimp_operation_layer_mode_blend_pin_light (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val;

              if (layer[c] > 0.5f)
                {
                  val = MAX(in[c], 2 * (layer[c] - 0.5));
                }
              else
                {
                  val = MIN(in[c], 2 * layer[c]);
                }
              comp[c] = val;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_screen (const gfloat *in,
                                        const gfloat *layer,
                                        gfloat       *comp,
                                        gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = 1.0f - (1.0f - in[c])   * (1.0f - layer[c]);
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_softlight (const gfloat *in,
                                           const gfloat *layer,
                                           gfloat       *comp,
                                           gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat multiply = in[c] * layer[c];
              gfloat screen   = 1.0f - (1.0f - in[c]) * (1.0f - layer[c]);
              gfloat val      = (1.0f - in[c]) * multiply + in[c] * screen;

              comp[c] = val;
            }
        }
      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

void
gimp_operation_layer_mode_blend_subtract (const gfloat *in,
                                          const gfloat *layer,
                                          gfloat       *comp,
                                          gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            comp[c] = in[c] - layer[c];
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}

/* added according to:
    http://www.simplefilter.de/en/basics/mixmods.html */
void
gimp_operation_layer_mode_blend_vivid_light (const gfloat *in,
                                             const gfloat *layer,
                                             gfloat       *comp,
                                             gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          gint c;

          for (c = 0; c < 3; c++)
            {
              gfloat val;

              if (layer[c] <= 0.5f)
                {
                  val = 1.0f - (1.0f - in[c]) / (2.0f * (layer[c]));
                }
              else
                {
                  val = in[c] / (2.0f * (1.0f - layer[c]));
                }
              val = MIN (val, 1.0f);

              comp[c] = val;
            }
        }

      comp[ALPHA] = layer[ALPHA];

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}


/*  subtractive blending functions.  these functions must set comp[ALPHA] to
 *  the modified alpha of the overlapping content, as a fraction of the
 *  original overlapping content (i.e., an alpha of 1.0 specifies that no
 *  content is subtracted.)  when in[ALPHA] or layer[ALPHA] are zero, the value
 *  of comp[RED..BLUE] is unconstrained (in particular, it may be NaN).
 */


void
gimp_operation_layer_mode_blend_color_erase (const gfloat *in,
                                             const gfloat *layer,
                                             gfloat       *comp,
                                             gint          samples)
{
  while (samples--)
    {
      if (in[ALPHA] != 0.0f && layer[ALPHA] != 0.0f)
        {
          const gfloat *color   = in;
          const gfloat *bgcolor = layer;
          gfloat       alpha;
          gint         c;

          alpha = 0.0f;

          for (c = 0; c < 3; c++)
            {
              gfloat col   = CLAMP (color[c],   0.0f, 1.0f);
              gfloat bgcol = CLAMP (bgcolor[c], 0.0f, 1.0f);

              if (col != bgcol)
                {
                  gfloat a;

                  if (col > bgcol)
                    a = (col - bgcol) / (1.0f - bgcol);
                  else
                    a = (bgcol - col) / bgcol;

                  alpha = MAX (alpha, a);
                }
            }

          if (alpha > 0.0f)
            {
              gfloat alpha_inv = 1.0f / alpha;

              for (c = 0; c < 3; c++)
                comp[c] = (color[c] - bgcolor[c]) * alpha_inv + bgcolor[c];
            }
          else
            {
              comp[RED] = comp[GREEN] = comp[BLUE] = 0.0f;
            }

          comp[ALPHA] = alpha;
        }
      else
        comp[ALPHA] = 0.0f;

      comp  += 4;
      layer += 4;
      in    += 4;
    }
}
