#pragma once

#include "../common/types.h"
#include "backend_connection.h"
#include "backend_querybuilder.h"
#include "backend_resultset.h"


namespace vtapi {

class IBackendInterface
{
public:
    /**
     * @brief Creates database connection object
     * @param connection_string connection string
     * @return new connection object
     */
    virtual Connection *createConnection(const std::string& connection_string) const = 0;

    /**
     * @brief Creates query building object
     * @param connection previously created database connection object
     * @param init_string init string (full query or default table)
     * @return new query building object
     */
    virtual QueryBuilder *createQueryBuilder(Connection &connection,
                                             const std::string& init_string) const = 0;

    /**
     * @brief Creates result set object
     * @param dbtypes database types object
     * @return  new result set object
     */
    virtual ResultSet *createResultSet(DBTYPES_MAP *dbtypes) const = 0;
};

}