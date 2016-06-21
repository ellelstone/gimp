/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpimage-convert-precision.c
 * Copyright (C) 2012 Michael Natterer <mitch@gimp.org>
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

#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gegl.h>

#include "libgimpcolor/gimpcolor.h"

#include "core-types.h"

#include "gegl/gimp-babl.h"

#include "gimpdrawable.h"
#include "gimpimage.h"
#include "gimpimage-color-profile.h"
#include "gimpimage-convert-precision.h"
#include "gimpimage-undo.h"
#include "gimpimage-undo-push.h"
#include "gimpprogress.h"
#include "gimpsubprogress.h"

#include "text/gimptextlayer.h"

#include "gimp-intl.h"


void
gimp_image_convert_precision (GimpImage     *image,
                              GimpPrecision  precision,
                              gint           layer_dither_type,
                              gint           text_layer_dither_type,
                              gint           mask_dither_type,
                              GimpProgress  *progress)
{
  GimpColorProfile *old_profile;
  GimpColorProfile *new_profile = NULL;
  //const Babl       *old_format;
  //const Babl       *new_format;
  GList            *all_drawables;
  GList            *list;
  const gchar      *undo_desc    = NULL;
  GimpProgress     *sub_progress = NULL;
  gint              nth_drawable, n_drawables;

  g_return_if_fail (GIMP_IS_IMAGE (image));
  g_return_if_fail (precision != gimp_image_get_precision (image));
  g_return_if_fail (precision == GIMP_PRECISION_U8_GAMMA ||
                    TRUE);
  g_return_if_fail (progress == NULL || GIMP_IS_PROGRESS (progress));

  all_drawables = g_list_concat (gimp_image_get_layer_list (image),
                                 gimp_image_get_channel_list (image));

  n_drawables = g_list_length (all_drawables) + 1 /* + selection */;

  if (progress)
    sub_progress = gimp_sub_progress_new (progress);

  switch (precision)
    {
    case GIMP_PRECISION_U8_GAMMA:
      undo_desc = C_("undo-type", "Convert Image to 8 bit integer");
      break;
    case GIMP_PRECISION_U16_GAMMA:
      undo_desc = C_("undo-type", "Convert Image to 16 bit integer");
      break;
    case GIMP_PRECISION_U32_GAMMA:
      undo_desc = C_("undo-type", "Convert Image to 32 bit integer");
      break;
    case GIMP_PRECISION_HALF_GAMMA:
      undo_desc = C_("undo-type", "Convert Image to 16 bit floating point");
      break;
    case GIMP_PRECISION_FLOAT_GAMMA:
      undo_desc = C_("undo-type", "Convert Image to 32 bit floating point");
      break;
    case GIMP_PRECISION_DOUBLE_GAMMA:
      undo_desc = C_("undo-type", "Convert Image to 64 bit floating point");
      break;
    }

  if (progress)
    gimp_progress_start (progress, FALSE, "%s", undo_desc);

  g_object_freeze_notify (G_OBJECT (image));

  gimp_image_undo_group_start (image, GIMP_UNDO_GROUP_IMAGE_CONVERT,
                               undo_desc);

  /*  Push the image precision to the stack  */
  gimp_image_undo_push_image_precision (image, NULL);

  old_profile = gimp_image_get_color_profile (image);
  new_profile = old_profile;/*elle: prevents unwanted ICC profile conversion*/

  /*  Set the new precision  */
  g_object_set (image, "precision", precision, NULL);

  for (list = all_drawables, nth_drawable = 0;
       list;
       list = g_list_next (list), nth_drawable++)
    {
      GimpDrawable *drawable = list->data;
      gint          dither_type;

      if (gimp_item_is_text_layer (GIMP_ITEM (drawable)))
        dither_type = text_layer_dither_type;
      else
        dither_type = layer_dither_type;

      if (sub_progress)
        gimp_sub_progress_set_step (GIMP_SUB_PROGRESS (sub_progress),
                                    nth_drawable, n_drawables);

      gimp_drawable_convert_type (drawable, image,
                                  gimp_drawable_get_base_type (drawable),
                                  precision,
                                  new_profile,
                                  dither_type,
                                  mask_dither_type,
                                  TRUE, sub_progress);
    }

  g_list_free (all_drawables);

  if (new_profile)
    {
      if (new_profile != old_profile)
        gimp_image_set_color_profile (image, new_profile, NULL);

      g_object_unref (new_profile);
    }

  /*  convert the selection mask  */
  {
    GimpChannel *mask = gimp_image_get_mask (image);
    GeglBuffer  *buffer;

    if (sub_progress)
      gimp_sub_progress_set_step (GIMP_SUB_PROGRESS (sub_progress),
                                  nth_drawable, n_drawables);

    gimp_image_undo_push_mask_precision (image, NULL, mask);

    buffer = gegl_buffer_new (GEGL_RECTANGLE (0, 0,
                                              gimp_image_get_width  (image),
                                              gimp_image_get_height (image)),
                              gimp_image_get_mask_format (image));

    gegl_buffer_copy (gimp_drawable_get_buffer (GIMP_DRAWABLE (mask)), NULL,
                      GEGL_ABYSS_NONE,
                      buffer, NULL);

    gimp_drawable_set_buffer (GIMP_DRAWABLE (mask), FALSE, NULL, buffer);
    g_object_unref (buffer);

    nth_drawable++;
  }

  if (sub_progress)
    gimp_progress_set_value (sub_progress, 1.0);

  gimp_image_undo_group_end (image);

  gimp_image_precision_changed (image);
  g_object_thaw_notify (G_OBJECT (image));

  if (sub_progress)
    g_object_unref (sub_progress);

  if (progress)
    gimp_progress_end (progress);
}
