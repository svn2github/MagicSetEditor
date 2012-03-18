//+----------------------------------------------------------------------------+
//| Description:  Magic Set Editor - Program to make Magic (tm) cards          |
//| Copyright:    (C) 2001 - 2010 Twan van Laarhoven and Sean Hunt             |
//| License:      GNU General Public License 2 or later (see file COPYING)     |
//+----------------------------------------------------------------------------+

// ----------------------------------------------------------------------------- : Includes

#include <util/prec.hpp>
#include <data/field/symbol.hpp>
#include <render/symbol/filter.hpp>

// ----------------------------------------------------------------------------- : SymbolField

IMPLEMENT_FIELD_TYPE(Symbol, "symbol");

IMPLEMENT_REFLECTION(SymbolField) {
	REFLECT_BASE(Field);
}


// ----------------------------------------------------------------------------- : SymbolStyle

IMPLEMENT_REFLECTION(SymbolStyle) {
	REFLECT_BASE(Style);
	REFLECT(min_aspect_ratio);
	REFLECT(max_aspect_ratio);
	REFLECT(variations);
}

SymbolVariation::SymbolVariation()
	: border_radius(0.05)
{}
SymbolVariation::~SymbolVariation() {}

bool SymbolVariation::operator == (const SymbolVariation& that) const {
	return name          == that.name
	    && border_radius == that.border_radius
	    && *filter       == *that.filter;
}

IMPLEMENT_REFLECTION_NO_SCRIPT(SymbolVariation) {
	REFLECT(name);
	REFLECT(border_radius);
	REFLECT_NAMELESS(filter);
}

// ----------------------------------------------------------------------------- : SymbolValue

#if USE_SCRIPT_VALUE_SYMBOL

ScriptValueP script_local_symbol_file(LocalFileName const& filename) {
	return intrusive(new LocalSymbolFile(filename));
}

String quote_string(String const& str);
String LocalSymbolFile::toCode() const {
	return _("local_symbol_file(") + quote_string(filename.toStringForWriting()) + _(")");
}
String LocalSymbolFile::typeName() const {
	return _("symbol");
}
GeneratedImageP LocalSymbolFile::toImage() const {
	SymbolVariationP variation(new SymbolVariation);
	variation->filter = intrusive(new SolidFillSymbolFilter);
	return intrusive(new SymbolToImage(true, filename, variation));
}
String LocalSymbolFile::toFriendlyString() const {
	return _("<") + _TYPE_("symbol") + _(">");
}

#else

IMPLEMENT_VALUE_CLONE(Symbol);

String SymbolValue::toFriendlyString() const {
	return filename.empty() ? wxEmptyString : _("<symbol>");
}

IMPLEMENT_REFLECTION_NO_GET_MEMBER(SymbolValue) {
	if (fieldP->save_value || !reflector.isWriting()) REFLECT_NAMELESS(filename);
}
void SymbolValue::reflect(GetMember&) {}
void SymbolValue::reflect(GetDefaultMember&) {}
#endif
