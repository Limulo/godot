/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"
#include "pd_player.h"

void register_libpd_types() {

        ObjectTypeDB::register_type<PDPlayer>();
}

void unregister_libpd_types() {
   //nothing to do here
}
