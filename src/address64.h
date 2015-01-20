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

#ifndef __GEEBEE_ADDRESS64_H__
#define __GEEBEE_ADDRESS64_H__

#include <glib.h>

G_BEGIN_DECLS

typedef struct _GeebeeAddress64 {
  guint32 msb;
  guint32 lsb;
} GeebeeAddress64;

typedef struct _GeebeeAddress16 {
  guint16 addr;
} GeebeeAddress16;

static const GeebeeAddress64 GEEBEE_BROADCAST_ADDR64 = {
  .msb = 0,
  .lsb = 0xffff
};

static const GeebeeAddress16 GEEBEE_BROADCAST_ADDR16 = {
  .addr = 0xfffe
};

G_END_DECLS

#endif // __GEEBEE_ADDRESS64_H__
