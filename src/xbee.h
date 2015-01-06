/**
 * Copyright (C) 2015 Torrie Fischer <tdfischer@phrobo.net>
 *
 * This file is part of GeeBee, the GLib XBee/ZigBee library.
 *
 * GeeBee is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GeeBee is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GeeBee. If not, see <http://www.gnu.org/licenses/>
 */

#ifndef __GEEBEE_XBEE_H__
#define __GEEBEE_XBEE_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "address64.h"
#include "request.h"

G_BEGIN_DECLS

#define GEEBEE_XBEE_TYPE            (geebee_xbee_get_type ())
#define GEEBEE_XBEE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEEBEE_XBEE_TYPE, GeebeeXbee))
#define GEEBEE_XBEE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GEEBEE_XBEE_TYPE, GeebeeXbeeClass))
#define IS_GEEBEE_XBEE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEEBEE_XBEE_TYPE))
#define IS_GEEBEE_XBEE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GEEBEE_XBEE_TYPE))
#define GEEBEE_XBEE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GEEBEE_XBEE_TYPE, GeebeeXbeeClass))

typedef struct _GeebeeXbee      GeebeeXbee;
typedef struct _GeebeeXbeeClass GeebeeXbeeClass;
typedef struct _GeebeeXbeePrivate GeebeeXbeePrivate;

struct _GeebeeXbeeClass
{
  GObjectClass parent_class;
};

struct _GeebeeXbee
{
  GObject parent;
  GeebeeXbeePrivate *priv;
};

GType geebee_xbee_get_type (void);

GeebeeXbee *geebee_xbee_new ();
void geebee_xbee_send_async_full (GeebeeXbee *xbee,
                                  GeebeePacket *pkt,
                                  GCancellable *cancellable,
                                  GAsyncReadyCallback sent_callback,
                                  gpointer sent_user_data);
void geebee_xbee_send_async (GeebeeXbee *xbee,
                             GeebeePacket *pkt,
                             GCancellable *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer user_data);

G_END_DECLS

#endif
