
#pragma once

#include <cstdint>

namespace FixedPoint {

class Fixed {
    public:
        using IntegerType = int32_t;
        using FractionType = int32_t;
        using InternalType = int32_t;
        using PrecisionType = int64_t;
        
        constexpr static const unsigned FRACTION_SIZE = 10;
        constexpr static const unsigned FRACTION_MASK = (1 << FRACTION_SIZE) - 1;

    public:
        constexpr Fixed(): _value(0) {}
        
        constexpr Fixed(const int& val) : _value(val << FRACTION_SIZE) {}
        constexpr Fixed(const unsigned int& val) : _value(val << FRACTION_SIZE) {}
        constexpr Fixed(const long int& val) : _value(val << FRACTION_SIZE) {}
        constexpr Fixed(const long unsigned int& val) : _value(val << FRACTION_SIZE) {}
        constexpr Fixed(const float& val) : _value(val * (1 << FRACTION_SIZE)) {}
        constexpr Fixed(const double& val) : _value(val * (1 << FRACTION_SIZE)) {}
        
        constexpr Fixed(const IntegerType& integer, const FractionType& fraction) : _value((integer << FRACTION_SIZE) + fraction) {}
        
        // Named constructors:
        constexpr static Fixed fromInternal(const InternalType& value) {
            return Fixed(InternalTypeWrapper(value));
        }
        
        constexpr IntegerType integer() const {
            return (_value >= 0) ? _value >> FRACTION_SIZE : -(-_value >> FRACTION_SIZE);
        }
        constexpr Fixed fraction() const {
            return fromInternal((_value >= 0) ? _value & FRACTION_MASK : (_value | ~FRACTION_MASK));
        }
        constexpr InternalType internal() const {
            return _value;
        }
        
        // Copy constructor
        constexpr Fixed(const Fixed& other) : _value(other._value) {}
        
        // Minus operator
        constexpr Fixed operator -() const {
            return fromInternal(-_value);
        }
        
        // Increment and decrement operators
        Fixed& operator ++() {
            ++_value;
            return *this;
        }
        Fixed& operator --() {
            --_value;
            return *this;
        }
        
        // Assignment operators
        Fixed& operator =(const Fixed& other) {
            _value = other._value;
            return *this;
        }
        Fixed& operator +=(const Fixed& other) {
            _value += other._value;
            return *this;
        }
        Fixed& operator -=(const Fixed& other) {
            _value -= other._value;
            return *this;
        }
        Fixed& operator *=(const Fixed& other) {
            PrecisionType product = _value * static_cast<PrecisionType>(other._value);
            _value = product >> FRACTION_SIZE;
            return *this;
        }
        Fixed& operator *=(const IntegerType& value) {
            _value *= value;
            return *this;
        }
        Fixed& operator /=(const Fixed& other) {
            PrecisionType num = static_cast<PrecisionType>(_value) << FRACTION_SIZE;
            _value = num / other._value;
            return *this;
        }
        Fixed& operator /=(const IntegerType& value) {
            _value /= value;
            return *this;
        }
    
    private:
        class InternalTypeWrapper {
        	public:
        		constexpr inline explicit InternalTypeWrapper(const InternalType& val) : _value(val) {}
        		constexpr inline explicit operator InternalType() const { return _value; }
        	
        	private:
        		const InternalType _value;
    	};
    	
        constexpr inline Fixed(const InternalTypeWrapper& val) : _value(static_cast<InternalType>(val)) {}
	    
        InternalType _value;
};


// Comparison Operators

constexpr bool operator ==(const Fixed& lhs, const Fixed rhs) {
    return (lhs.internal() == rhs.internal());
}

constexpr bool operator !=(const Fixed& lhs, const Fixed rhs) {
    return (lhs.internal() != rhs.internal());
}

constexpr bool operator <(const Fixed& lhs, const Fixed rhs) {
    return (lhs.internal() < rhs.internal());
}

constexpr bool operator >(const Fixed& lhs, const Fixed rhs) {
    return (lhs.internal() > rhs.internal());
}

constexpr bool operator <=(const Fixed& lhs, const Fixed rhs) {
    return (lhs.internal() <= rhs.internal());
}

constexpr bool operator >=(const Fixed& lhs, const Fixed rhs) {
    return (lhs.internal() >= rhs.internal());
}


// Arithmetic Operators

constexpr Fixed operator +(const Fixed& lhs, const Fixed& rhs) {
    return Fixed::fromInternal(lhs.internal() + rhs.internal());
}

template<typename RhsType>
constexpr Fixed operator +(const Fixed& lhs, const RhsType& rhs) {
    return Fixed::fromInternal(lhs.internal() + (rhs << Fixed::FRACTION_SIZE));
}

template<typename LhsType>
constexpr Fixed operator +(const LhsType& lhs, const Fixed& rhs) {
    return Fixed::fromInternal((lhs << Fixed::FRACTION_SIZE) + rhs.internal());
}

constexpr Fixed operator -(const Fixed& lhs, const Fixed& rhs) {
    return Fixed::fromInternal(lhs.internal() - rhs.internal());
}

template<typename RhsType>
constexpr Fixed operator -(const Fixed& lhs, const RhsType& rhs) {
    return Fixed::fromInternal(lhs.internal() - (rhs << Fixed::FRACTION_SIZE));
}

template<typename LhsType>
constexpr Fixed operator -(const LhsType& lhs, const Fixed& rhs) {
    return Fixed::fromInternal((lhs << Fixed::FRACTION_SIZE) - rhs.internal());
}

constexpr Fixed operator *(const Fixed& lhs, const Fixed& rhs) {
    return Fixed::fromInternal((static_cast<Fixed::PrecisionType>(lhs.internal()) * rhs.internal()) >> Fixed::FRACTION_SIZE);
}

template<typename rhsType>
constexpr Fixed operator *(const Fixed& lhs, const rhsType& rhs) {
    return Fixed::fromInternal(lhs.internal() * rhs);
}

template<typename rhsType>
constexpr Fixed operator *(const rhsType& lhs, const Fixed& rhs) {
    return Fixed::fromInternal(lhs * rhs.internal());
}

constexpr Fixed operator /(const Fixed& lhs, const Fixed& rhs) {
    return Fixed::fromInternal((static_cast<Fixed::PrecisionType>(lhs.internal()) << Fixed::FRACTION_SIZE) / rhs.internal());
}

template<typename RhsType>
constexpr Fixed operator /(const Fixed& lhs, const RhsType& rhs) {
    return Fixed::fromInternal(lhs.internal() / rhs);
}

} // namespace FixedPoint
