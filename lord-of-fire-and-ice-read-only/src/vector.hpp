/**
 * vector.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_VECTOR_HPP
#define GRAPHICSLAB_VECTOR_HPP

// Includes
#include "defs.hpp"
#include "exception.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/**
 * Vector template class
 */
template<uint S, typename T> // Size and type of elements
class Vector
{
private:
    /** Array of elements */
    T _array[S];

public:
    /** Constructor */
    Vector() {}
    
    /**
     * Constructor.
     *
     * @param[in]   t   All the vector elements are initialized to given parameter value
     */
    Vector( T t )
    {
        for ( uint i = 0; i < S; i++ )
            _array[i] = t;
    }
    
    /**
     * Constructor.
     *
     * @param[in]   tArray  Vector elements are initialized from the given array
     */
    Vector( T tArray[] )
    {
        for ( uint i = 0; i < S; i++ )
            _array[i] = tArray[i];
    }
        
    /**
     * Constructor.
     * Useful for 3 element vectors.
     *
     * @param[in]   t1      First element of the vector
     * @param[in]   t2      Second element of the vector
     * @param[in]   t3      Third element of the vector
     */     
    Vector( const T &t1, const T &t2, const T &t3 )
    {
        if ( S >= 3 )
        {
            _array[0] = t1;
            _array[1] = t2;
            _array[2] = t3;
        }
        else
        {
            throw Exception( "Vector has less than three elements to initialize" );
        }
    }
    
        
    /**
     * Copy constructor
     *
     * @param[in]   other   Vector of the same type to copy from
     */
    Vector( const Vector<S, T> &other )
    {
        for ( uint i = 0; i < S; i++ )
        {
            _array[i] = other[i];
        }
    }
    
    // Functions to get first, second and third values
    // which are x, y and z in a vector.
    T &x() const
    { 
        if ( S < 1 )
            throw Exception( "Index out of bounds" );
        return const_cast<T&>( _array[0] ); 
    }
    T &y() const
    { 
        if ( S < 2 )
            throw Exception( "Index out of bounds" );
        return const_cast<T&>( _array[1] ); 
    }
    T &z() const
    { 
        if ( S < 3 )
            throw Exception( "Index out of bounds" );
        return const_cast<T&>( _array[2] ); 
    }
    
    
    /**
     * Operator overloading for [] to get the element at index.
     *
     * @param[in]   index   Index of the element to get
     *
     * @return      Reference to the element at given index
     */
    T &operator[]( uint index ) const 
    {
        if ( index < S )
            return const_cast<T&>( _array[index] );
        
        throw Exception( "Index out of bounds" );
    }
    

    /**
     * Operator overloading for addition of two vectors of the same type.
     *
     * @param[in]   other   Vector to add with
     *
     * @return      A new vector which is addition of this and other vector
     */     
    Vector<S, T> operator +( const Vector<S, T> &other ) const
    {
        Vector<S, T> result;
        for ( uint i = 0; i < S; i++ )
            result._array[i] = _array[i] + other._array[i];
        
        return result;
    }

    
    /**
     * Operator overloading for subtraction of two vectors of the same type.
     *
     * @param[in]   other   Vector to subtract from this
     *
     * @return      A new vector which is subtraction of other from this vector
     */
    Vector<S, T> operator -( const Vector<S, T> &other ) const
    {
        Vector<S, T> result;
        for ( uint i = 0; i < S; i++ )
            result._array[i] = _array[i] - other._array[i];
        
        return result;
    }

    
    /**
     * Operator overloading for addition of two vectors of the same type.
     *
     * @param[in]   other   Vector to add to this vector
     */
    void operator +=( const Vector<S, T> &other )
    {
        for ( uint i = 0; i < S; i++ )
        {
            _array[i] += other._array[i];
        }
    }
    
    
    /**
     * Operator overloading for subtraction of two vectors of the same type.
     *
     * @param[in]   other   Vector to subtract from this vector
     */
    void operator -=( const Vector<S, T> &other )
    {
        for ( uint i = 0; i < S; i++ )
        {
            _array[i] -= other._array[i];
        }
    }
    
    
    /**
     * Operator overloading for multiplication with a scalar, for scaling.
     *
     * @param[in]   t   Scalar to multiply with
     */
    void operator *= ( const T &t )
    {
        for ( uint i = 0; i < S; i++ )
            _array[i] *= t;
    }
    
    
    /**
     * Operator overloading for division with a scalar, for scaling.
     *
     * @param[in]   t   Scalar to divide with
     */
    void operator /= ( const T &t )
    {
        for ( uint i = 0; i < S; i++ )
            _array[i] /= t;
    }
    
    const T *array() const { return _array; }
    
    
}; // class Vector


/** Typedefinitions for Vector template */
typedef Vector<2, float> vec2f;
typedef Vector<3, float> vec3f;
typedef Vector<3, int> vec3i;
typedef Vector<3, uint> vec3ui;
typedef Vector<3, uchar> vec3ub;
typedef Vector<4, uint> vec4ui;


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_VECTOR_HPP


