/**
 * @file
 * @authors
 * VTApi Team, FIT BUT, CZ
 * Petr Chmelar, chmelarp@fit.vutbr.cz
 * Vojtech Froml, xfroml00@stud.fit.vutbr.cz
 * Tomas Volf, ivolf@fit.vutbr.cz
 *
 *
 * @section LECENSE License
 *
 * There will be license information for VTApi.
 * &copy; FIT BUT, CZ, 2011
 *
 *
 * @section DESCRIPTION Description
 *
 * Commons classes which provide a auxiliary functionality of VTApi.
 *
 */

#ifndef INTERNALS_H
#define	INTERNALS_H

// Using this you can allow using OpenCV
// #define _OpenCV

#ifdef _OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif


#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include "postgresql/libpqtypes.h"
#include "vtapi_settings.h"

typedef std::string String;
#define BUFFERSize 255

// be nice while destructing
#define destruct(v) if (v) { delete(v); (v) = NULL; }
#define destructall(v) if (v) { delete[](v); (v) = NULL; }
// #define boolStr(b) ((b) ? "true" : "false")

// format: 0=text, 1=binary
#define PGF 1

class TypeMap;
class Logger;
class Connector;

/**
 * @brief Standard logger
 * 
 * Allows to log to a cerr or to a file if specified...
 *
 * // log("Testing the logging first.");
 * 2011-09-30 19:34:20: Testing the logging first.
 *
 * Programmers may use also write (just it) and a conditional debug log.
 * @note Error codes 10*
 *
 *//***************************************************************************/
class Logger {
// Members
protected:
    String logFilename;  /**< A name of the file for storing logs */
    std::ofstream logStream;  /**< A stream of file for storing logs */

// Methods
public:
    /**
     * Construct a logger object
     * @param filename Name of the file for storing logs
     */
    Logger(const String& filename = "");
    /**
     * Destroy the logger
     */
    virtual ~Logger();

    /**
     * Log function puts a timestamp with the message into the logstream
     * @param message message, which to be logged
     */
    void log(const String& message);
    /**
     * Debug function flush a timestamp with the message immediately into the logstream
     * @param message
     */
    void debug(const String& message);

    /**
     * This is to write to the standard error log
     * @param message
     */
    void write(const String& message);
    /**
     * Get a timestamp for logging
     * @return string value with timestamp
     * @note Timestamp format is "YYYY-MM-DD HH:NN:SS", where NN represents minutes
     */
    String timestamp();
};


/**
 * @brief Maintains connection to the database
 * 
 * Based on a connection string supplied of the form:
 *   Connector("host=localhost port=5432 dbname=db user=postgres password='secret'", new Logger());
 * The class requires a Logger too.
 *
 * @note Error codes 12*
 *//***************************************************************************/
class Connector {
// Members
public:
// protected:
    String  conninfo; /**< connection string to access the database */
    PGconn* conn;     /**< handler of the current database connection */
    Logger* logger;   /**< logger, in which we record messages */

// Methods
public:
    Connector(const Connector& orig);

    /**
     * This is the fine constructor
     * @param connectionInfo connection string to access the database
     * @param logger logger, in which we record messages
     */
    Connector(const String& connectionInfo, Logger* logger = NULL);

    /**
     * Close the connection to the database and destroy a connector
     */
    virtual ~Connector();

    /**
     * YES, this is a fun that (re)connects to the database
     * @param connectionInfo connection string to the database
     * @return success
     */
    bool reconnect(const String& connectionInfo);
    
    /**
     * SELECT and log version of the database :)
     * @return success
     */
    bool connected();

    /**
     * Get logger of the connector
     * @return logger of the connector
     */
    Logger* getLogger();
    /**
     * Get handler of the current database connection
     * @return handler of the current database connection
     */
    PGconn* getConn();      // connection
};



/**
 * @brief This is common predecessor to each object in the VTApi - manages connection, logging and error handling.
 *
 * This class is inherited by many and many other classes, but it manages just single resources,
 * thus there may bee a doom if someone destroys the original ones. Well, destructor should only happen
 * when isDoom is false, which is set by the only constructor: Commons(String connStr); .
 *
 * @note Error codes 15*
 *//***************************************************************************/
class Commons {
// Members
public:
    String thisClass; /**< This should be filled in each constructor of a derived class */

    Connector* connector; /**< This was most probably inherited */
    Logger* logger; /**< Current logger*/

    bool verbose; /**< Verbose mode - write used SQL queries and commands */
    enum format_t {STANDARD, CSV, HTML} format; /**< Output format */
    String input; /**< A filename from which we read an input */
    String output; /**< A filename into which we write an output */
    String user; /**< Logged in user */
    String baseLocation; /**< Base path location @todo */

    String dataset;          /**< Current dataset name */
    String datasetLocation;  /**< Current dataset location */
    String sequence;         /**< Current sequence name */
    String sequenceLocation; /**< Current sequence location */

    std::pair<int,int> interval; /**< Current interval from<->to (startTime <-> EndTime) */
    String method;     /**< Current method name */
    String process;    /**< Current process name */
    String selection;  /**< Current selection name */

    TypeMap* typemap;  /**< Bidirectional map with oid <-> typname (datatype) */

    bool doom; /**< every derived class will have +1 = (true :) */
    
// Methods
public:
    /**
     * This is OK for most of applications if there are some Commons or derived classes.
     * The default constructor should never exist - Commons();
     * @param orig 
     * @todo parameter orig in documentation
     */
    Commons(const Commons& orig); // hmm, much better

    /**
     * This is OK for most of applications if there are some Commons or derived classes.
     * The default constructor should never exist - Commons();
     * @param other 
     * @todo parameter other in documentation
     */
    Commons(Connector& other); // hmm, much better

    /**
     * A custom API startup constructor
     * This construtors should not cause any doom
     * @param connStr
     * @param location
     * @param user
     * @param password
     * @param logFilename
     * @todo NOT IMPLEMENTED?
     */
    // Commons(const String& connStr, const String& logFilename = "");
    Commons(const String connStr, const String location, const String user, const String password, const String logFilename = "");

    /**
     * And a shorter one...
     */
    Commons(const gengetopt_args_info& args_info);

    /**
     * This should be called from any (virtual) constructor of a derived class
     *     this->beDoomed();
     */
    void beDoomed();

    /**
     * This constructor is close to doom as capitalism;
     * this is different from comunism, which has been destroyed already
     */
    virtual ~Commons();

    /**
     * This causes a serious death
     * @param errnum number of the error
     * @param logline error message to display
     */
    void error(int errnum, const String& logline);
    /**
     * This causes a serious death
     * @param message error message to display
     */
    void error(const String& message);

    /**
     * This is just a warning
     * @param errnum number of the warning
     * @param logline warning message to display
     */
    void warning(int errnum, const String& logline);
    /**
     * This is just a warning
     * @param message warning message to display
     */
    void warning(const String& message);

    /**
     *
     * @param format
     * @todo FUTURE - NOT IMPLEMENTED
     */
    void read(const String& format="");

    // some functions that may be usefull
    /**
     * Get current connector
     * @return connector
     */
    Connector* getConnector();
    /**
     * Get current logger
     * @return logger
     */
    Logger* getLogger();

    /**
     * Get name of current dataset
     * @return dataset name
     */
    String getDataset();
    /**
     * Get name of current sequence
     * @return sequence name
     */
    String getSequence();
    /**
     * Get name of current selection
     * @return selection name
     */
    String getSelection();

    /**
     * Get data location path
     * @return data location
     */
    String getDataLocation();

    /**
     * Convert entered datatype to an OID number
     * @param typname name of the datatype
     * @return OID number, which represents entered datatype
     */
    int toOid(String typname);
    /**
     * Convert entered oid to a datatype name
     * @param oid OID number, which represents some datatype
     * @return name of the datatype
     */
    String toTypname(int oid);

    /**
     * Load datatypes from PostgreSQL and register to the VTApi
     */
    void registerTypes();
};

/**
 * @brief Auxiliary class which holds maping between OID an typname of data type
 */
class TypeMap {
// Members
private:
    std::map<int, String> oid2typname; /**< Pair of int, datatype to get a datatype from the integer representation */
    std::map<String, int> typname2oid; /**< Pair of datatype, int to get an integer representation from the datatype */
public:
    bool dataloaded;

// Methods
public:
    /**
     * Construct a bidirectional map
     */
    TypeMap();
    /**
     * Destruct the bidirectional map
     */
    virtual ~TypeMap();

    /**
     * Clear the bidirectional map - size of the map will be 0
     */
    void clear();
    /**
     * Determines whether the bidirectional map is empty (has size 0)
     * @return emptiness of the bidirectional map
     */
    bool empty();
    /**
     * Insert a pair of <int, datatype> to bidirectional map
     * @param oid OID of the datatype
     * @param typname name of the datatype
     */
    void insert(int oid, String typname);
    /**
     * Insert a pair of <datatype, int> to bidirectional map
     * @param typname name of the type
     * @param oid OID of the type
     */
    void insert(String typname, int oid);
    /**
     * Get a size of the bidirectional map
     * @return size of the bidirectional map
     */
    int size();

    /**
     * Convert entered datatype to an OID number
     * @param typname name of the datatype
     * @return OID number, which represents entered datatype
     */
    int toOid(String typname);
    /**
     * Convert entered oid to a datatype name
     * @param Oid oid number, which represents some datatype
     * @return name of the datatype
     */
    String toTypname(int Oid);
};


/**
 * A generic function to convert any numeric type to string
 * (any numeric type, e.g. int, float, double, etc.)
 * @param t
 * @return string
 */
template <class T>
inline String toString(const T& t) {
    std::stringstream strstr;
    strstr << t;
    return strstr.str();
};



#endif	/* INTERNALS_H */