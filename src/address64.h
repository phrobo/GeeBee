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

#include <glib.h>

#ifndef __GEEBEE_ADDRESS64_H__
#define __GEEBEE_ADDRESS64_H__

G_BEGIN_DECLS

struct GeebeeAddress64 {
  guint32 msb;
  guint32 lsb;
};

struct GeebeeAddress16 {
  guint16 addr;
};

#endif // __GEEBEE_ADDRESS64_H__
