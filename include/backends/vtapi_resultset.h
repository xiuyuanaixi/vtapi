/* 
 * File:   vtapi_resultset.h
 * Author: vojca
 *
 * Created on May 7, 2013, 12:40 PM
 */

#ifndef VTAPI_RESULTSET_H
#define	VTAPI_RESULTSET_H

namespace vtapi {
    class ResultSet;
    class PGResultSet;
    class SLResultSet;
}

#include "vtapi_libloader.h"
#include "vtapi_typemanager.h"
#include "../common/vtapi_tkey.h"
#include "../common/vtapi_logger.h"
#include "../common/vtapi_global.h"

namespace vtapi {


typedef struct {
    int     rows;
    int     cols;
    char    **res;
} sl_res_t;


//TODO: comment
class ResultSet {
protected:

    func_map_t      *FUNC_MAP;      /**< function address book */
    TypeManager     *typeManager;   /**< object for type manipulation */
    Logger          *logger;        /**< logger object for output messaging */
    string          thisClass;      /**< class name */

    int             pos;            /**< position within resultset */
    void            *res;           /**< result object */

public:

    ResultSet(func_map_t *FUNC_MAP, TypeManager *typeManager, Logger *logger) {
        this->logger = logger;
        this->typeManager = typeManager;
        this->FUNC_MAP = FUNC_MAP;
        this->pos = -1;
        this->res = NULL;
    };
    virtual ~ResultSet() { };

    virtual void newResult(void *res) = 0;

    void step() { this->pos++; };
    int getPosition() { return this->pos; };
    void setPosition(const int pos) { this->pos = pos; };

    virtual int countRows() = 0;
    virtual int countCols() = 0;
    virtual bool isOk() = 0;
    virtual void clear() = 0;

   /**
     * Get key of a single table column
     * @param col Column index
     * @return Column key
     */
    virtual TKey getKey(int col) = 0;
    /**
     * Get a list of all possible columns as TKeys
     * @return list of keys
     */
    virtual TKeys *getKeys() = 0;

    // =============== GETTERS (Select) ========================================

     /**
     * Generic getter - fetches any value from resultset and returns it as string
     * @param col column index
     * @param arrayLimit array length to print
     * @return string representation of field value
     */
    virtual string getValue(const int col, const int arrayLimit = 0) = 0;

    // =============== GETTERS FOR CHAR, CHAR ARRAYS AND STRINGS ===============
    /**
     * Get single character specified by a column key
     * @param key column key
     * @return character
     */
    char getChar(const string& key) {
        return this->getChar(this->getKeyIndex(key));
    }
    /**
     * Get single character specified by column index
     * @param pos column index
     * @return character
     */
    virtual char getChar(const int col) = 0;
    /**
     * Get character array specified by column key
     * @param key column key
     * @param size size of the array of char values
     * @return character array
     */
    char *getCharA(const string& key, int& size) {
        return this->getCharA(this->getKeyIndex(key), size);
    };
    /**
     * Get character array specified by column index
     * @param pos column index
     * @param size size of the array of char values
     * @return character array
     */
    virtual char *getCharA(const int pos, int& size) = 0;
    /**
     * Get a character array specified by a column key
     * @param key column key
     * @return character array
     */
    string getString(const string& key) {
        return this->getString(this->getKeyIndex(key));
    };
    /**
     * Get a string value specified by an index of a column
     * @param col column index
     * @return string value
     */
    virtual string getString(const int col) = 0;

    // =============== GETTERS FOR INTEGERS OR ARRAYS OF INTEGERS ==============
    /**
     * Get an integer value specified by a column key
     * @param key column key
     * @return integer value
     */
    int getInt(const string& key) {
        return this->getInt(this->getKeyIndex(key));
    };
    /**
     * Get an integer value specified by an index of a column
     * @param col index of column
     * @return integer value
     */
    virtual int getInt(const int col) = 0;
    /**
     * Get long integer value specified by a column key
     * @param key column key
     * @return long integer value
     */
    long getInt8(const string& key) {
        return this->getInt8(this->getKeyIndex(key));
    };
    /**
     * Get long integer value specified by an index of a column
     * @param col index of column
     * @return long integer value
     */
    virtual long getInt8(const int col) = 0;
    /**
     * Get an array of integer values specified by a column key
     * @param key column key
     * @param size size of the array of integer values
     * @return array of integer values
     */
    int *getIntA(const string& key, int& size) {
        return this->getIntA(this->getKeyIndex(key), size);
    };
    /**
     * Get an array of integer values specified by an index of a column
     * @param col index of column
     * @param size size of the array of integer values
     * @return array of integer values
     */
    virtual int *getIntA(const int col, int& size) = 0;
    /**
     * Get a vector of integer values specified by a column key
     * @param key column key
     * @return vector of integer values
     */
    vector<int> *getIntV(const string& key) {
        return this->getIntV(this->getKeyIndex(key));
    };
    /**
     * Get a vector of integer values specified by an index of a column
     * @param col index of column
     * @return vector of integer values
     */
    virtual vector<int> *getIntV(const int col) = 0;
    /**
     * Get a vector of integer vectors specified by a column key
     * @param key column key
     * @return  vector of vectors of integer values
     */
    vector< vector<int>* > *getIntVV(const string& key) {
        return this->getIntVV(this->getKeyIndex(key));
    };
    /**
     * Get a vector of integer vectors specified by an index of a column
     * @param col index of column
     * @return  vector of vectors of integer values
     */
    virtual vector< vector<int>* > *getIntVV(const int col) = 0;
    

    // =============== GETTERS FOR FLOATS OR ARRAYS OF FLOATS ==================
    /**
     * Get a float value specified by a column key
     * @param key column key
     * @return float value
     */
    float getFloat(const string& key) {
        return this->getFloat(this->getKeyIndex(key));
    };
    /**
     * Get a float value specified by an index of a column
     * @param col index of column
     * @return float value
     */
    virtual float getFloat(const int col) = 0;
    /**
     * Get a double value specified by a column key
     * @param key column key
     * @return double value
     */
    double getFloat8(const string& key) {
        return this->getFloat8(this->getKeyIndex(key));
    };
    /**
     * Get a double value specified by an index of a column
     * @param col index of column
     * @return double value
     */
    virtual double getFloat8(const int col) = 0;
    /**
     * Get an array of float values specified by a column key
     * @param key column key
     * @param size size of the array of float values
     * @return array of float values
     */
    float* getFloatA(const string& key, int& size) {
        return this->getFloatA(this->getKeyIndex(key), size);
    };
    /**
     * Get array of float values specified by index of column
     * @param col index of column
     * @param size size of the array of float values
     * @return array of float values
     */
    virtual float *getFloatA(const int col, int& size) = 0;
    /**
     * Get a vector of float values specified by the column index
     * @param key column key
     * @return vector of float values
     */
    vector<float> *getFloatV(const string& key) {
        return this->getFloatV(this->getKeyIndex(key));
    };
    /**
     * Get a vector of integer values specified by column key
     * @param col index of column
     * @return vector of integer values
     */
    virtual vector<float> *getFloatV(const int col) = 0;
    

    //TODO: is it needed a vector of float vectors as in case of integers?

    // =============== GETTERS - TIMESTAMP =====================================
    /**
     * Get timestamp specified by column key
     * @param key column key
     * @return Timestamp info
     */
    time_t getTimestamp(const string& key) {
        return this->getTimestamp(this->getKeyIndex(key));
    };
    /**
     * Get timestamp specified by the column index
     * @param col column index
     * @return Timestamp info
     */
    virtual time_t getTimestamp(const int col) = 0;

    // =============== GETTERS - OpenCV MATRICES ===============================
#ifdef __OPENCV_CORE_C_H__
//    /**
//     * Get OpenCv matrix (cvMat) specified by the column key
//     * @param key column key
//     * @return CvMat structure
//     */
//    CvMat *getCvMat(const string& key);
//    /**
//     * Get OpenCv matrix (cvMat) specified by the column index
//     * @param col column index
//     * @return CvMat structure
//     */
//    CvMat *getCvMat(const int col);
//    /**
//     * Get OpenCv matrix (cvMatND) specified by the column key
//     * @param key column key
//     * @return CvMatND structure
//     */
//    CvMatND *getCvMatND(const string& key);
//    /**
//     * Get OpenCv matrix (cvMatND) specified by the column index
//     * @param col column index
//     * @return CvMatND structure
//     */
//    CvMatND *getCvMatND(const int col);
#endif

    // =============== GETTERS - GEOMETRIC TYPES ===============================
#ifdef POSTGIS
//    /**
//     * Get 2D point specified by the column key
//     * @param key column key
//     * @return 2D Point
//     */
//    PGpoint getPoint(const string& key);
//    /**
//     * Get 2D point specified by the column index
//     * @param col column index
//     * @return 2D Point
//     */
//    PGpoint getPoint(const int col);
//    /**
//     * Get line segment specified by the column key
//     * @param key column key
//     * @return Line segment
//     */
//    PGlseg getLineSegment(const string& key);
//    /**
//     * Get line segment specified by the column index
//     * @param col column index
//     * @return Line segment
//     */
//    PGlseg getLineSegment(const int col);
//    /**
//     * Get box specified by the column key
//     * @param key column key
//     * @return Box
//     */
//    PGbox getBox(const string& key);
//    /**
//     * Get box specified by the column index
//     * @param col column index
//     * @return Box
//     */
//    PGbox getBox(const int col);
//    /**
//     * Get circle specified by the column key
//     * @param key column key
//     * @return Circle
//     */
//    PGcircle getCircle(const string& key);
//    /**
//     * Get circle specified by the column index
//     * @param col column index
//     * @return Circle
//     */
//    PGcircle getCircle(const int col);
//    /**
//     * Get polygon specified by the column key
//     * @note polygon.pts must be copied out if needed after clearing resultset
//     *          copy_points(polygon.npts, polygon.pts, ...);
//     * @param key column key
//     * @return Polygon
//     */
//    PGpolygon getPolygon(const string& key);
//    /**
//     * Get polygon specified by the column index
//     * @note polygon.pts must be copied out if needed after clearing resultset
//     *          copy_points(polygon.npts, polygon.pts, ...);
//     * @param col column index
//     * @return Polygon
//     */
//    PGpolygon getPolygon(const int col);
//    /**
//     * Get path specified by the column key
//     * @note path.pts must be copied out if needed after clearing resultset
//     *          copy_points(path.npts, path.pts, ...);
//     * @param key column key
//     * @return Path
//     */
//    PGpath getPath(const string& key);
//    /**
//     * Get path specified by the column index
//     * @note path.pts must be copied out if needed after clearing resultset
//     *          copy_points(path.npts, path.pts, ...);
//     * @param col column index
//     * @return Path
//     */
//    PGpath getPath(const int col);
//
//    /**
//     * Get cube specified by the column index
//     * @note Cube is defined by 1 (= point) or 2 (= opposite corners of cube) points
//     * @note Points may have 1-100(CUBE_MAX_DIM) dimensions
//     * @param key column key
//     * @return Cube
//     */
//    PGcube getCube(const string& key);
//    /**
//     * Get cube specified by the column key
//     * @note Cube is defined by 1 (= point) or 2 (= opposite corners of cube) points
//     * @note Points may have 1-100(CUBE_MAX_DIM) dimensions
//     * @param col column index
//     * @return Cube
//     */
//    PGcube getCube(const int col);
//
//    /**
//     * Get GEOS geometry type by the column key
//     * @param key column key
//     * @return GEOS geometry
//     */
//    GEOSGeometry* getGeometry(const string& key);
//    /**
//     * Get GEOS geometry type by the column index
//     * @param col column index
//     * @return GEOS geometry
//     */
//    GEOSGeometry* getGeometry(const int col);
//    /**
//     * Get GEOS geometry (linestring) type by the column key
//     * @param key column key
//     * @return GEOS geometry
//     */
//    GEOSGeometry* getLineString(const string& key);
//    /**
//     * Get GEOS geometry (linestring) type by the column index
//     * @param col column index
//     * @return GEOS geometry
//     */
//    GEOSGeometry* getLineString(const int col);
//    /**
//     * Get array of 2D points specified by the column index
//     * @param key column index
//     * @return vector of 2D Points
//     */
//    vector<PGpoint>*  getPointV(const string& key);
//    /**
//     * Get array of 2D points specified by the column key
//     * @param col column key
//     * @return vector of 2D Points
//     */
//    vector<PGpoint>*  getPointV(const int col);
#endif

    // =============== GETTERS - OTHER =========================================
    /**
     * Get an integer with an OID value specified by a column key
     * @param key column key
     * @return integer with the OID value
     */
    int getIntOid(const string& key) {
        return this->getIntOid(this->getKeyIndex(key));
    };
    /**
     * Get an integer with an OID value specified by a column index
     * @param col column index
     * @return integer with the OID value
     */
    virtual int getIntOid(const int col) = 0;

    ////////////// Print support methods
    /**
     * This goes through resultset and retrieves metadata necessary for print.
     * @note It needs to be done before every print.
     * @param row if not set to -1, this indicates single row print
     * @param get_widths whether column widths will be required @todo @b doc[VF]: který popisek je správný (tento nebo "desired column widths" z .cpp)?
     * @return metadata for print, pair consisting of two vectors:
     *  a) Tkeys - column types etc., b) ints - column widths
     */
    virtual pair< TKeys*,vector<int>* > getKeysWidths(const int row, bool get_widths, const int arrayLimit) = 0;

protected:

    virtual string getKeyType(const int col) = 0;
    virtual int getKeyIndex(const string& key) = 0;

};


class PGResultSet : public ResultSet {
public:

    PGResultSet(func_map_t *FUNC_MAP, TypeManager *typeManager, Logger *logger);
    ~PGResultSet();

    void newResult(void *res);

    int countRows();
    int countCols();
    bool isOk();
    void clear();

    TKey getKey(int col);
    TKeys* getKeys();

    string getValue(const int col, const int arrayLimit = 0);

    char getChar(const int pos);
    char *getCharA(const int pos, int& size);
    string getString(const int col);
    int getInt(const int col);
    long getInt8(const int col);
    int* getIntA(const int col, int& size);
    vector<int>* getIntV(const int col);
    vector< vector<int>* >* getIntVV(const int col);
    float getFloat(const int col);
    double getFloat8(const int col);
    float* getFloatA(const int col, int& size);
    vector<float>* getFloatV(const int col);
    time_t getTimestamp(const int col);
    int getIntOid(const int col);

    pair< TKeys*,vector<int>* > getKeysWidths(const int row = -1, bool get_widths = 1, const int arrayLimit = 0);

protected:

    string getKeyType(const int col);
    int getKeyIndex(const string& key);

};


class SLResultSet : public ResultSet {
public:

    SLResultSet(func_map_t *FUNC_MAP, TypeManager *typeManager, Logger *logger);
    ~SLResultSet();

    void newResult(void *res);

    int countRows();
    int countCols();
    bool isOk();
    void clear();

    TKey getKey(int col);
    TKeys* getKeys();

    string getValue(const int col, const int arrayLimit = 0);

    char getChar(const int pos);
    char *getCharA(const int pos, int& size);
    string getString(const int col);
    int getInt(const int col);
    long getInt8(const int col);
    int* getIntA(const int col, int& size);
    vector<int>* getIntV(const int col);
    vector< vector<int>* >* getIntVV(const int col);
    float getFloat(const int col);
    double getFloat8(const int col);
    float* getFloatA(const int col, int& size);
    vector<float>* getFloatV(const int col);
    time_t getTimestamp(const int col);
    int getIntOid(const int col);

    pair< TKeys*,vector<int>* > getKeysWidths(const int row = -1, bool get_widths = 1, const int arrayLimit = 0);

protected:

    string getKeyType(const int col);
    int getKeyIndex(const string& key);

};

} // namespace vtapi

#endif	/* VTAPI_RESULTSET_H */
