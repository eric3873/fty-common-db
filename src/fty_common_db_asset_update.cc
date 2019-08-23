/*  =========================================================================
    fty_common_db_asset_update - Functions updating assets in database.

    Copyright (C) 2014 - 2018 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

/*
@header
    fty_common_db_asset_update - Functions updating assets in database.
@discuss
@end
*/

#include <tntdb/row.h>
#include <tntdb/result.h>
#include <tntdb/error.h>

#include <fty_common.h>

#include "fty_common_db_classes.h"

#define ERRCODE_ABNORMAL 1

namespace DBAssetsUpdate {

int
update_asset_element (tntdb::Connection &conn,
                      uint32_t element_id,
                      const char *element_name,
                      uint32_t parent_id,
                      const char *status,
                      uint16_t priority,
                      const char *asset_tag,
                      int32_t &affected_rows)
{
    LOG_START;
    log_debug ("  element_id = %" PRIi32, element_id);
    log_debug ("  element_name = '%s'", element_name);
    log_debug ("  parent_id = %" PRIu32, parent_id);
    log_debug ("  status = '%s'", status);
    log_debug ("  priority = %" PRIu16, priority);
    log_debug ("  asset_tag = '%s'", asset_tag);

    // if parent id == 0 ->  it means that there is no parent and value
    // should be updated to NULL
    try{
        tntdb::Statement st = conn.prepare(
            " UPDATE"
            "   t_bios_asset_element"
            " SET"
//            "   name = :name,"
            "   asset_tag = :asset_tag,"
            "   id_parent = :id_parent,"
            "   status = :status,"
            "   priority = :priority"
            " WHERE id_asset_element = :id"
        );

        st = st.set("id", element_id).
//                           set("name", element_name).
                           set("status", status).
                           set("priority", priority).
                           set("asset_tag", asset_tag);

        if ( parent_id != 0 )
        {
            affected_rows = st.set("id_parent", parent_id).
                               execute();
        }
        else
        {
            affected_rows = st.setNull("id_parent").
                               execute();
        }
        log_debug("[t_asset_element]: updated %" PRIu32 " rows", affected_rows);
        LOG_END;
        // if we are here and affected rows = 0 -> nothing was updated because
        // it was the same
        return 0;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return ERRCODE_ABNORMAL;
    }
}
} // namespace end
