#include "zend.h"
#include "zend_API.h"
#include "zend_attributes.h"

ZEND_API zend_class_entry *zend_ce_php_attribute;
ZEND_API zend_class_entry *zend_ce_php_compiler_attribute;

static HashTable internal_validators;

void zend_attribute_validate_phpattribute(zend_attribute *attr, int target)
{
	if (target != ZEND_ATTRIBUTE_TARGET_CLASS) {
		zend_error(E_COMPILE_ERROR, "Only classes can be marked with <<PhpAttribute>>");
	}
}

void zend_attribute_validate_phpcompilerattribute(zend_attribute *attr, int target)
{
	zend_error(E_COMPILE_ERROR, "The PhpCompilerAttribute can only be used by internal classes, use PhpAttribute instead");
}

ZEND_API void zend_register_attribute_ce(void)
{
	zend_hash_init(&internal_validators, 8, NULL, NULL, 1);

	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "PhpAttribute", NULL);
	zend_ce_php_attribute = zend_register_internal_class(&ce);
	zend_ce_php_attribute->ce_flags |= ZEND_ACC_FINAL;

	zend_compiler_attribute_register(zend_ce_php_attribute, zend_attribute_validate_phpattribute);

	INIT_CLASS_ENTRY(ce, "PhpCompilerAttribute", NULL);
	zend_ce_php_compiler_attribute = zend_register_internal_class(&ce);
	zend_ce_php_compiler_attribute->ce_flags |= ZEND_ACC_FINAL;

	zend_compiler_attribute_register(zend_ce_php_compiler_attribute, zend_attribute_validate_phpcompilerattribute);
}

ZEND_API zend_attributes_internal_validator zend_attribute_get_validator(zend_string *lcname)
{
	return zend_hash_find_ptr(&internal_validators, lcname);
}

ZEND_API void zend_compiler_attribute_register(zend_class_entry *ce, zend_attributes_internal_validator validator)
{
	zend_string *lcname = zend_string_tolower_ex(ce->name, 1);

	zend_hash_update_ptr(&internal_validators, lcname, validator);
	zend_string_release(lcname);
}
