/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2008 CRI Middleware, Inc. All rights reserved.
 *
 * Use, modification and distribution are subject to the CRI Script Software
 * License, Version 1.0(see accompanying file "CriScript_License_1_0.txt" or
 * copy at www.criscript.com/trac/wiki/CRI%20Script%20Software%20License%201.0 ).
 *
 *
 * Library  : CRIScript
 * Module   : JScript Variable definitions
 * File     : variable.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

namespace cri {

/***************************************************************************
 *      Constants
 ***************************************************************************/
/*EN
 toString precision for float
*/
const int32_t TOSTRING_PRECISION_SINGLE = 6;
/*EN
 toString precision for double
*/
const int32_t TOSTRING_PRECISION_DOUBLE = 12;

/***************************************************************************
 *      Enums
 ***************************************************************************/
/*EN
 Primitive type pereference values when retrieving the variable's primitive type.
 \ingroup MDL_LIB_VM
*/
enum PRIMITIVETYPE_PREFERENCE {
	PRIMITIVETYPE_DEFAULT,	/*EN< Default type*/
	PRIMITIVETYPE_NUMBER,	/*EN< Prefers a number type*/
	PRIMITIVETYPE_STRING,	/*EN< Prefers a string type*/
	PRIMITIVETYPE_PREFERRED,
};

/***************************************************************************
 *      Variable slot info
 ***************************************************************************/
class CVmObject;

/*EN
 Pointer to the CVmObject.
 \ingroup MDL_LIB_VM
*/
typedef CVmObject* VMOBJECTREF ;

	
	/*EN
	 \brief Class represent a variable in the CRI Sript VM
	 \ingroup MDL_LIB_VM
	*/
	class CVariable {
	public:
		/*EN ctor, No argument version*/
		CVariable();

		/*EN ctor, with an initializer
		\param i int32_t initializer
		*/
		CVariable( const int32_t i );

		/*EN ctor, with an initializer
		\param i64 int64_t initializer
		*/
		CVariable( const int64_t i64 );

		/*EN ctor, with an initializer
		\param f float initializer
		*/
		CVariable( const float f );

		/*EN ctor, with an initializer
		\param d double initializer
		*/
		CVariable( const double d );

		/*EN ctor, with an initializer
		\param ref VM Ojbect reference initializer
		*/
		CVariable( const VMOBJECTREF ref );

		/*EN ctor, with an initializer
		\param str Pointer to wstring initializer
		*/
		CVariable( wstring* const str );

		/*EN ctor, with an initializer
		\param str Pointer to wchar_t buffer
		*/
		CVariable( const wchar_t * const str );

		/*EN ctor, with an initializer
		\param b bool initializer
		*/
		CVariable( const bool b );

		/*EN ctor, with an initializer and with a variable flag setting
		\param i int32_t initializer
		\param flag operand flags
		*/
		CVariable( const int32_t i, const OPERAND_FLAG flag );

		/*EN ctor, with an initializer and with a variable flag setting
		\param i64 int64_t initializer
		\param flag operand flags
		*/
		CVariable( const int64_t i64, const OPERAND_FLAG flag  );

		/*EN ctor, with an initializer and with a variable flag setting
		\param f float initializer
		\param flag operand flags
		*/
		CVariable( const float f, const OPERAND_FLAG flag  );

		/*EN ctor, with an initializer and with a variable flag setting
		\param d double initializer
		\param flag operand flags
		*/
		CVariable( const double d, const OPERAND_FLAG flag  );

		/*EN ctor, with an initializer and with a variable flag setting
		\param ref VM Object reference initializer
		\param flag operand flags
		*/
		CVariable( const VMOBJECTREF ref, const OPERAND_FLAG flag  );

		/*EN ctor, with an initializer and with a variable flag setting
		\param str Pointer to wstirng initializer
		\param flag operand flags
		*/
		CVariable( wstring* const str, const OPERAND_FLAG flag  );

		/*EN ctor, with an initializer and with a variable flag setting
		\param str Pointer to wchar_t buffer
		\param flag operand flags
		*/
		CVariable( const wchar_t * const str , const OPERAND_FLAG flag  );

		/*EN ctor, with an initializer and with a variable flag setting
		\param b boolean initializer
		\param flag operand flags
		*/
		CVariable( const bool b, const OPERAND_FLAG flag );

		/*EN ctor, with a flag initializer
		\param flag operand flags
		*/
		CVariable( const OPERAND_FLAG flag );
		
		/*EN ctor, with a variable initializer
		\param var value operand. value type and values are copied to the dest. Flags are cleared.
		*/
		CVariable( const CVariable& var );	//Copy constructor

		/*EN Copy ctor
		Increments ref count if the value is an obect ref.
		*/
		CVariable& operator=( const CVariable& var );	//Operator

		/*EN dtor
		Decrements ref count if the value is an obect ref.
		*/
		~CVariable();


		/*EN Fast copy routine for eval stack operation
		*/
		void inline copyFast( const CVariable& src )
		{
			iOperandType = (OPERAND_TYPE)src.iOperandType;

			switch( OperandType( src.iOperandType ) )
			{
			case OPERAND_DOUBLE:
			case OPERAND_INT64:
			case OPERAND_UNSIGNEDINT64:
				i64Value = src.i64Value;
				return;
			default:
				iValue = src.iValue;
				return;
			case OPERAND_OBJECTREF:
				refObject = src.refObject;
				addObjectRef();
				return;
			case OPERAND_STRING:
				pString = new wstring( *src.pString );
				return;
			}
		};

		/*EN Internal helper to erase variable contents if it keeps a string or object referense.
		After the call, the variable is safe to forget
		*/
		void erase();
		
		/*EN Internal helper to add obj referenece
		*/
		void addObjectRef();

		/*EN Sets an object reference to the variable.
		\param ref VM Object reference to be set
		This call will increment ref count of the target VM object.
		*/
		void setObjectRef( const VMOBJECTREF ref );

		/*EN Sets an object reference to the variable with flags.
		\param ref VM Object reference to be set
		\param flag object flags
		\param ridConstraint a type of the object type constraint if the object has some constraint type. 
		This call will increment ref count of the target VM object.
		*/
		void setObjectRefWithFlags( const VMOBJECTREF ref, const OPERAND_FLAG flag, const RID ridConstraint );

		/*EN Sets a string to the variable.
		\param str a reference to wstring instance
		*/
		void setString( const wstring& str );

		/*EN Sets an integer to the variable.
		\param i an integer to be set
		*/
		void setInt( const int32_t i );

		/*EN Sets an integer64 to the variable.
		\param i an integer64 value to be set
		*/
		void setInt64( const int64_t i );

		/*EN Sets a float to the variable.
		\param i a float value to be set
		*/
		void setFloat( const float f );

		/*EN Sets a double to the variable.
		\param i a double value to be set
		*/
		void setDouble( const double d );

		/*EN Sets a string to the variable.
		\param str Pointer to wstring instance
		*/
		void setStringPointer( wstring* str );

		/*EN Sets flags to the variable.
		\param flag flags
		*/
		void setFlag( const OPERAND_FLAG flag );

		/*EN Converts the variable to a string and retrieves a result.
		\param pVm VM instance pointer. a string conversion requires VM instances to execute.
		\return Pointer to a converted string. The pointer need to be deleted by the caller.
		*/
		wstring* toString( CCilVm* pVm );

		/*EN Converts the variable to a string value.
		\param pVm VM instance pointer. a string conversion requires VM instances to execute.
		*/
		void convertToString( CCilVm* pVm );

		/*EN Converts the variable to a string and retrieves a result with specified radix.
		 \param pVm VM instance pointer. a string conversion requires VM instances to execute.
		 \param iRadix Radix to be used while conversion
		 \return Pointer to a converted string. The pointer need to be deleted by the caller.
		 */
		wstring* toString( CCilVm* pVm, const int32_t iRadix );
		
		/*EN Converts the variable to a string value with specified radix.
		 \param pVm VM instance pointer. a string conversion requires VM instances to execute.
		 \param iRadix Radix to be used while conversion
		 */
		void convertToString( CCilVm* pVm, const int32_t iRadix );

		/*EN Converts the variable to a integer and retrieves a result.
		\return Convesion result.
		*/
		int32_t toInt( void );

		/*EN Converts the variable to a 64bit integer and retrieves a result.
		\return aConvesion result.
		*/
		int64_t toInt64( void );

		/*EN Converts the variable to a float and retrieves a result.
		\return a convesion result.
		*/
		float toFloat( void );

		/*EN Converts the variable to a double and retrieves a result.
		\return Convesion result.
		*/
		double toDouble( void );
		/*EN Converts the variable to a boolean and retrieves a result.
		\return Convesion result.
		*/
		bool toBoolean( void );

		/*EN Copies a variable to the destination. This API call is slightly faster for string and object pointer.
		Source variable content is destructed.
		*/
		void moveTo( CVariable& dest );

		/*EN Copies a variable to the destination. This API call is slightly faster for string and object pointer.
		Source variable content is destructed.
		This version also duplicate flags
		*/
		void moveToWithFlags( CVariable& dest );

		/*EN Retrieves a primitive type of the variable.
		\return Primitive Type of the variable
		\param prefferred Prefferred value of the primitive type. Some object can have both Number and String primitive types.
		*/
		OPERAND_TYPE getPrimitiveType( PRIMITIVETYPE_PREFERENCE prefferred = PRIMITIVETYPE_DEFAULT );
		
		/*EN Converts a variable to a primitive type.
		\param pVm A VM instance pointer. a string conversion requires VM instances to execute.
		*/
		void convertToPrimitiveType( CCilVm* pVm );

		/*EN Retrieves flags of the variable.
		\return flags
		*/
		OPERAND_FLAG getFlag() { return (OPERAND_FLAG)iOperandFlag; }

		/*EN Retrieves a stricted type of VM Object ref.
		\return stricted RID
		*/
		RID			GetConstraintRid()
		{
			//Since ridConstraintType is 24bit bitfield, we need to supply object type flag
			return ridConstraintType | MDT_TYPEDEF;
		}

		//--------------------------------------------------
		//operators
		//--------------------------------------------------
		operator float()	{ return toFloat(); }	/*EN< float cast*/
		operator double()	{ return toDouble(); }	/*EN< double cast*/
		operator bool()		{ return toBoolean(); }	/*EN< boolean cast*/
		operator int()		{ return toInt(); }		/*EN< integer cast*/
		operator int64_t()	{ return toInt64(); }	/*EN< 64bit integer cast*/

		union {
			struct {
				uint32_t		iOperandType	:4;		/*EN< operand type*/
				uint32_t		iOperandFlag	:4;		/*EN< operand flag*/
				RID				ridConstraintType	:24;	/*EN< object type constraint type*/
			};
			int32_t iPrameters;
		};
		/*EN An union of values*/
		union {
			bool	bValue;					/*EN< boolean value/union*/
			float	fValue;					/*EN< float value/union*/
			double	dValue;					/*EN< double value/union*/
			int32_t iValue;					/*EN< integer value/union*/
			int64_t i64Value;				/*EN< 64bit integer value/union*/
			VMOBJECTREF refObject;			/*EN< object reference pointer/union*/
			wstring*pString;				/*EN< wstring pointer/union*/
			void*	pPointerInternal;		/*EN< internally used pointer/union*/
		};
	protected:
		void cleanupFloatString( wstring& str );
	};
}	//namespace cri

