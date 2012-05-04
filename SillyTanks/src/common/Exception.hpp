/**
 * exception.hpp
 *
 */

#ifndef GRAPHICSLAB_EXCEPTION_HPP
#define GRAPHICSLAB_EXCEPTION_HPP


// Global definitions include
#include "Definitions.hpp"

// Includes
#include <string>


GAME_NAMESPACE_BEGIN


/**
 * Generic exception class
 */
class Exception
{
public:
    /**
     * Constructor
     *
     * @param[in]   error   Exception error string
     */
    Exception( const std::string &error ) : _error( error ) {}
    
    /** Destructor */
    virtual ~Exception() throw() {}
    
    /**
     * Get the exception error string.
     *
     * @return      Exception error string
     */
    virtual const std::string &what() const throw()  { return _error; }
    
protected:
    /** Exception error string */
    std::string _error;
    
}; // class Exception


GAME_NAMESPACE_END


#endif // GRAPHICSLAB_EXCEPTION_HPP
