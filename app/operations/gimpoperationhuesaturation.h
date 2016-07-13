/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpoperationhuechroma.h
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

#ifndef __GIMP_OPERATION_HUE_CHROMA_H__
#define __GIMP_OPERATION_HUE_CHROMA_H__


#include "gimpoperationpointfilter.h"


#define GIMP_TYPE_OPERATION_HUE_CHROMA            (gimp_operation_hue_chroma_get_type ())
#define GIMP_OPERATION_HUE_CHROMA(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_OPERATION_HUE_CHROMA, GimpOperationHueChroma))
#define GIMP_OPERATION_HUE_CHROMA_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GIMP_TYPE_OPERATION_HUE_CHROMA, GimpOperationHueChromaClass))
#define GIMP_IS_OPERATION_HUE_CHROMA(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_OPERATION_HUE_CHROMA))
#define GIMP_IS_OPERATION_HUE_CHROMA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GIMP_TYPE_OPERATION_HUE_CHROMA))
#define GIMP_OPERATION_HUE_CHROMA_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GIMP_TYPE_OPERATION_HUE_CHROMA, GimpOperationHueChromaClass))


typedef struct _GimpOperationHueChroma      GimpOperationHueChroma;
typedef struct _GimpOperationHueChromaClass GimpOperationHueChromaClass;

struct _GimpOperationHueChroma
{
  GimpOperationPointFilter  parent_instance;
};

struct _GimpOperationHueChromaClass
{
  GimpOperationPointFilterClass  parent_class;
};


GType   gimp_operation_hue_chroma_get_type (void) G_GNUC_CONST;

void    gimp_operation_hue_chroma_map      (GimpHueChromaConfig *config,
                                                const GimpRGB           *color,
                                                GimpHueRange             range,
                                                GimpRGB                 *result);


#endif /* __GIMP_OPERATION_HUE_CHROMA_H__ */
