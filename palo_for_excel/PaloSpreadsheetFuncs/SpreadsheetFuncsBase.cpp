////////////////////////////////////////////////////////////////////////////////
/// @brief
///
/// @file
///
/// Copyright (C) 2006-2011 Jedox AG
///
/// This program is free software; you can redistribute it and/or modify it
/// under the terms of the GNU General Public License (Version 2) as published
/// by the Free Software Foundation at http://www.gnu.org/copyleft/gpl.html.
///
/// This program is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
/// more details.
///
/// You should have received a copy of the GNU General Public License along with
/// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
/// Place, Suite 330, Boston, MA 02111-1307 USA
///
/// You may obtain a copy of the License at
///
/// <a href="http://www.jedox.com/license_palo_bi_suite.txt">
///   http://www.jedox.com/license_palo_bi_suite.txt
/// </a>
///
/// If you are developing and distributing open source applications under the
/// GPL License, then you are free to use Palo under the GPL License.  For OEMs,
/// ISVs, and VARs who distribute Palo with their products, and do not license
/// and distribute their source code under the GPL, Jedox provides a flexible
/// OEM Commercial License.
///
/// @author Marek Pikulski (<a href="mailto:marek.pikulski@jedox.com">marek.pikulski@jedox.com</a>)
////////////////////////////////////////////////////////////////////////////////

// Additional Revisions to Palo (c) Junction BI LLC:
// JMM 09-20-13 For 5.0 library compatability

#include <sstream>

#include <libconnectionpool/StringUtils.h>

#include <libpalo_ng/Network/NetInitialisation.h>
#include <libpalo_ng/Palo/Database.h>
#include <libpalo_ng/Palo/Dimension.h>
#include <libpalo_ng/Palo/Cache/DatabaseCache.h>
#include <libpalo_ng/Util/StringUtils.h>

#include <PaloSpreadsheetFuncs/SpreadsheetFuncsBase.h>
#include <PaloSpreadsheetFuncs/StringArrayArray.h>
#include <PaloSpreadsheetFuncs/SpreadsheetFuncsException.h>

#include "StringTokenizer.h"
#include <boost/lexical_cast.hpp>

using namespace Palo::SpreadsheetFuncs;
using namespace Palo::Util;
using namespace std;
using namespace jedox::palo;

namespace Palo {
	namespace SpreadsheetFuncs {
		class PaloHelper {
		public:
			static const ELEMENT_INFO & findDimensionElementAt(Dimension d, const unsigned int offset ) {
				for ( std::unique_ptr<DimensionCache::CacheIterator> i = d.getIterator(); !( *i ).end(); ( *i )++ ) {
					if (( *i )->position == offset ) {
						return **i;
					}
				}

				throw SpreadsheetFuncsException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_NOT_FOUND );
			}

			static const ELEMENT_INFO & findDimensionElementAt(Dimension d, const unsigned int offset, const bool down ) {
				unsigned long curpos, bestpos = 0;
				long id = -1;

				if (down) {
					bestpos = (unsigned long)-1;
				};

				for ( std::unique_ptr<DimensionCache::CacheIterator> i = d.getIterator(); !( *i ).end(); ( *i )++ ) {
					curpos = ( *i )->position;
					if (curpos == offset ) {
						id = ( *i )->element;
						break;
					} else if (down && (curpos > offset) && (curpos <= bestpos)) {
						id = ( *i )->element;
						bestpos = curpos;
					} else if (!down && (curpos < offset) && (curpos >= bestpos)) {
						id = ( *i )->element;
						bestpos = curpos;
					}
				}

				if (id != -1) {
					return d[id].getCacheData();
				}

				throw SpreadsheetFuncsException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_NOT_FOUND );
			}
		};		
	}
}

NetInitialisation& NetInit = NetInitialisation::instance();

unsigned short SpreadsheetFuncsBase::backwards_startindex = 1;
const std::string SpreadsheetFuncsBase::doublepoint(":");
const std::string SpreadsheetFuncsBase::atsign("@");
const std::string SpreadsheetFuncsBase::emptystring("");
const std::string SpreadsheetFuncsBase::prefixpropertycube("#_CELL_PROPERTIES_");
const size_t SpreadsheetFuncsBase::lengthprefixpropertycube = SpreadsheetFuncsBase::prefixpropertycube.length();


const std::string SpreadsheetFuncsBase::strNoValueYet(MSG_NO_VALUE_YET);

SpreadsheetFuncsBase::SpreadsheetFuncsBase( ConnectionPool& cp ) : cp( cp ), qc(cp) {
}

SpreadsheetFuncsBase::~SpreadsheetFuncsBase() {}

void SpreadsheetFuncsBase::ChangeToNewStartIndex() {
	SpreadsheetFuncsBase::backwards_startindex = 0;
}

void SpreadsheetFuncsBase::InitSSL(std::string trustFile) {
	NetInit.initSSL(trustFile);
}

void SpreadsheetFuncsBase::calculationBegin() {
	clearedDimensions.clear();
}

void SpreadsheetFuncsBase::calculationEnd() {}

void SpreadsheetFuncsBase::FPaloCubeClear( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const ElementListArray& a ) {
	StringArrayArray sa;
	Database db = ( *s )[database];
	CUBE_INFO ci = ( *s )[database].cube[cube].getCacheData();

	for ( ElementListArray::const_iterator i = a.begin(); i < a.end(); i++ ) {
		if ( i->all() ) {
			sa.push_back( FPaloDimensionListElementsSimple( s, database, db.dimension[ci.dimensions[i-a.begin()]].getCacheData().ndimension ) );
		} else {
			sa.push_back( i->getArray() );
		}
	}

	( *s )[database].cube[cube].clear( sa );
}

void SpreadsheetFuncsBase::FPaloCubeRename( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const std::string& name ) {
	( *s )[database].cube[cube].rename( name );
}

CubeInfo SpreadsheetFuncsBase::FPaloCubeInfo( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube) {
	CubeInfo ci;
	CUBE_INFO info;
	( *s )[database].cube[cube].ActualInfo(info);	

	ci.identifier = info.cube;
	ci.name = info.ncube;
	ci.number_dimensions = info.number_dimensions;

	size_t i, len = ci.number_dimensions;
	ci.dimensions.reserve(len);

	Database db = ( *s )[database];
	for(i=0; i<len; i++) {
		ci.dimensions.push_back( db.dimension[info.dimensions[i]].getCacheData().ndimension );
	}

	ci.number_cells = info.number_cells;
	ci.number_filled_cells = info.number_filled_cells;
	ci.status = CubeInfo::CubeStatus(info.status);
	ci.type = CubeInfo::CubeType(info.type);

	if ((ci.type == CubeInfo::ATTRIBUTE) && isPropertyCube(ci.name)) {
		ci.type = CubeInfo::SYSTEM;
	}

	return ci;
}

StringArray SpreadsheetFuncsBase::FPaloDimensionInfo( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension) {
	StringArray sa;
	const DIMENSION_INFO info = ( *s )[database].dimension[dimension].getCacheData();
	sa.push_back( boost::lexical_cast<std::string>( info.dimension) );
	sa.push_back( info.ndimension );
	sa.push_back( boost::lexical_cast<std::string>( info.number_elements ) );
	sa.push_back( boost::lexical_cast<std::string>( info.maximum_level ) );
	sa.push_back( boost::lexical_cast<std::string>( info.maximum_indent ) );
	sa.push_back( boost::lexical_cast<std::string>( info.maximum_depth ) );
	sa.push_back( boost::lexical_cast<std::string>( info.type ) );
	sa.push_back( boost::lexical_cast<std::string>( info.assoc_dimension ) );
	sa.push_back( boost::lexical_cast<std::string>( info.attribute_cube ) );
	sa.push_back( boost::lexical_cast<std::string>( info.rights_cube ) );
	return sa;
}

void SpreadsheetFuncsBase::FPaloCubeClear( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube ) {
	( *s )[database].cube[cube].clear();
}


void SpreadsheetFuncsBase::FPaloDimensionClear( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	( *s )[database].dimension[dimension].clear();
}


void SpreadsheetFuncsBase::FPaloRootAddDatabase( boost::shared_ptr<Server> s, const std::string& database, const DATABASE_INFO::TYPE type ) {
	s->createDatabase( database, type);
}


bool SpreadsheetFuncsBase::FPaloRootDeleteDatabase( boost::shared_ptr<Server> s, const std::string& database ) {
	return ( *s )[database].destroy();
}


void SpreadsheetFuncsBase::FPaloDatabaseAddCube( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const StringArray& dimension_names ) {
	( *s )[database].createCube( cube, dimension_names );
}


void SpreadsheetFuncsBase::FPaloDatabaseAddDimension( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	( *s )[database].createDimension( dimension );
}


bool SpreadsheetFuncsBase::FPaloDatabaseDeleteCube( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube ) {
	return ( *s )[database].cube[cube].destroy();
}


bool SpreadsheetFuncsBase::FPaloDatabaseDeleteDimension( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	return ( *s )[database].dimension[dimension].destroy();
}


bool SpreadsheetFuncsBase::FPaloDatabaseLoadCube( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube ) {
	return ( *s )[database].cube[cube].load();
}


bool SpreadsheetFuncsBase::FPaloDatabaseUnloadCube( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube ) {
	return ( *s )[database].cube[cube].unload();
}


ConsolidationElementInfoArray SpreadsheetFuncsBase::FPaloElementListConsolidationElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	ConsolidationElementInfoArray cia;

	Dimension d = ( *s )[database].dimension[dimension];
	const ELEMENT_INFO& ei = d[element].getCacheData();

	cia.reserve( ei.number_children );

	for ( unsigned int i = 0; i < ei.number_children; i++ ) {
		try {
			const ELEMENT_INFO& cei = d[ei.children[i]].getCacheData();
			cia.push_back( ConsolidationElementInfo( cei.nelement, DimensionElementType( cei.type ), ei.children[i], ei.weights[i] ) );
		} catch ( const PaloException& ){}
	}

	return cia;
}

void SpreadsheetFuncsBase::FPaloElementListAncestorsHelper( std::set<DimensionElementInfoSimple>& vec, Dimension& dim, const ELEMENT_LIST& elements )
{
	size_t size=elements.size();
	for(size_t i=0;i<size;i++)
	{		
		const ELEMENT_INFO& ei = dim[elements[i]].getCacheData();

		vec.insert( DimensionElementInfoSimple( ei.nelement, DimensionElementType( ei.type ), ei.element ) );
		if(ei.children.size() > 0) 
		{
			FPaloElementListAncestorsHelper(vec, dim, ei.parents);
		} 	
	}
}

DimensionElementInfoSimpleArray SpreadsheetFuncsBase::FPaloElementListAncestors( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	std::set<DimensionElementInfoSimple> cia;
	
	Dimension dim = ( *s )[database].dimension[dimension];
	FPaloElementListAncestorsHelper(cia, dim, dim[element].getCacheData().parents);	
	return DimensionElementInfoSimpleArray(cia.begin(), cia.end());
}

void SpreadsheetFuncsBase::FPaloElementListDescendantsHelper( std::set<DimensionElementInfoSimple>& vec, Dimension& dim, const ELEMENT_LIST& elements )
{
	size_t size=elements.size();
	for(size_t i=0;i<size;i++)
	{		
		const ELEMENT_INFO& ei = dim[elements[i]].getCacheData();

		vec.insert( DimensionElementInfoSimple( ei.nelement, DimensionElementType( ei.type ), ei.element ) );
		if(ei.children.size() > 0) 
		{
			FPaloElementListDescendantsHelper(vec, dim, ei.children);
		} 
	}
}

DimensionElementInfoSimpleArray SpreadsheetFuncsBase::FPaloElementListDescendants( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	std::set<DimensionElementInfoSimple> cia;
	Dimension dim = ( *s )[database].dimension[dimension];	
	FPaloElementListDescendantsHelper(cia, dim, dim[element].getCacheData().children);	
	return DimensionElementInfoSimpleArray(cia.begin(), cia.end());;
}

DimensionElementInfoSimpleArray SpreadsheetFuncsBase::FPaloElementListParents( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	DimensionElementInfoSimpleArray cia;

	Dimension dim = ( *s )[database].dimension[dimension];
	const ELEMENT_LIST& parents = dim[element].getCacheData().parents;
	
	size_t size = parents.size();
	for(size_t i=0;i<size;i++) {		
		try {
			const ELEMENT_INFO ei = dim[parents[i]].getCacheData();
			cia.push_back(  DimensionElementInfoSimple( ei.nelement, DimensionElementType( ei.type ), ei.element )  );
		} catch ( const PaloException& ){}
	}
	return cia;
}

DimensionElementInfoSimpleArray SpreadsheetFuncsBase::FPaloElementListSiblings( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element) {
	DimensionElementInfoSimpleArray cia;
	
	Dimension dim = ( *s )[database].dimension[dimension];
	
	UINT level = dim[element].getCacheData().level;
		
	for ( std::unique_ptr<DimensionCache::CacheIterator> i = dim.getIterator(); !( *i ).end(); ( *i )++ ) 
	{
		if((**i).level == level) 
		{
			const ELEMENT_INFO& ei = (**i);
			cia.push_back(  DimensionElementInfoSimple( ei.nelement, DimensionElementType( ei.type ), ei.element )  );
		}
	}
	return cia;
}

struct erase_if_id_zero_helper 
{
	erase_if_id_zero_helper() {}

	const bool operator()(const DimensionElementInfo& i ) const {
		return i.identifier == -1;
	}

	const bool operator()(const DimensionElementInfoSimple& i ) const {
		return i.identifier == -1;
	}

};

size_t SpreadsheetFuncsBase::FPaloDimensionTopElementsCount( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension) {
	return ( *s )[database].dimension[dimension].getTopCount();
}


std::unique_ptr<DimensionElementInfoSimpleArray> SpreadsheetFuncsBase::FPaloDimensionSimpleListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long id, long start, long limit) {
	unique_ptr<DimensionElementInfoSimpleArray> a;

	a.reset( new DimensionElementInfoSimpleArray() );

	std::list<ELEMENT_INFO> elems = ( *s )[database].dimension[dimension].getElements(id, start, limit);
	size_t i=0, elemcount = elems.size();

	a->resize( elemcount);

	std::list<ELEMENT_INFO>::iterator it = elems.begin();
	std::list<ELEMENT_INFO>::iterator endlist = elems.end();

	while ( (i < elemcount) && (it != endlist)) {
		(*a)[i].identifier = it->element;
		(*a)[i].name = it->nelement;
		(*a)[i].type = it->type;
		i++;
		++it;
	}

	return a;
}

std::unique_ptr<DimensionElementInfoSimpleArray> SpreadsheetFuncsBase::FPaloDimensionSimpleFlatListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long start, long limit) {
	return FPaloDimensionSimpleListElements(s, database, dimension, -2, start, limit);
}

std::unique_ptr<DimensionElementInfoSimpleArray> SpreadsheetFuncsBase::FPaloDimensionSimpleTopListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long start, long limit) {
	return FPaloDimensionSimpleListElements(s, database, dimension, -1, start, limit);
}

std::unique_ptr<DimensionElementInfoSimpleArray> SpreadsheetFuncsBase::FPaloDimensionSimpleChildrenListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long element_identifier, long start, long limit) {
	return FPaloDimensionSimpleListElements(s, database, dimension, element_identifier, start, limit);
}

std::unique_ptr<DimensionElementInfoReducedArray> SpreadsheetFuncsBase::FPaloDimensionReducedListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long id, long start, long limit) {
	unique_ptr<DimensionElementInfoReducedArray> a;

	a.reset( new DimensionElementInfoReducedArray() );

	std::list<ELEMENT_INFO> elems = ( *s )[database].dimension[dimension].getElements(id, start, limit);
	size_t i=0, elemcount = elems.size();

	a->resize( elemcount);

	std::list<ELEMENT_INFO>::iterator it = elems.begin();
	std::list<ELEMENT_INFO>::iterator endlist = elems.end();

	while ( (i < elemcount) && (it != endlist)) {
		(*a)[i] = *it;
		i++;
		++it;
	}

	return a;
}

std::unique_ptr<DimensionElementInfoReducedArray> SpreadsheetFuncsBase::FPaloDimensionReducedFlatListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long start, long limit) {
	return FPaloDimensionReducedListElements(s, database, dimension, -2, start, limit);
}

std::unique_ptr<DimensionElementInfoReducedArray> SpreadsheetFuncsBase::FPaloDimensionReducedTopListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long start, long limit) {
	return FPaloDimensionReducedListElements(s, database, dimension, -1, start, limit);
}

std::unique_ptr<DimensionElementInfoReducedArray> SpreadsheetFuncsBase::FPaloDimensionReducedChildrenListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long element_identifier, long start, long limit) {
	return FPaloDimensionReducedListElements(s, database, dimension, element_identifier, start, limit);
}

std::unique_ptr<DimensionElementInfoArray> SpreadsheetFuncsBase::FPaloDimensionListElements2( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, bool filter ) {
	unique_ptr<DimensionElementInfoArray> a;

	a.reset( new DimensionElementInfoArray() );

	// some php user forget to execute palo_ping after last palo_eadd
	s->ping();

	Dimension d = ( *s )[database].dimension[dimension];
	const DIMENSION_INFO& di = d.getCacheData();

	a->resize( di.number_elements );

	for ( std::unique_ptr<DimensionCache::CacheIterator> i = d.getIterator(); !( *i ).end(); ( *i )++ ) {
		const ElementCache& ec = **i;
		DimensionElementInfo& dei = ( *a )[ec.position];

		dei.depth = ec.depth;
		dei.identifier = ec.element;
		dei.indent = ec.indent;
		dei.level = ec.level;
		dei.name = ec.nelement;
		dei.type = DimensionElementType( ec.type );
		dei.position = ec.position;
		//dei.locked = ec.lock;
	}

	for ( std::unique_ptr<DimensionCache::CacheIterator> i = d.getIterator(); !( *i ).end(); ++( *i ) ) {
		const ElementCache& ec = **i;
		DimensionElementInfo& dei = ( *a )[ec.position];

		dei.children.reserve( ec.number_children );
		for ( unsigned int i = 0; i < ec.number_children; i++ ) {
			try {
				dei.children.push_back( ConsolidationElementInfo( ( DimensionElementInfoSimple )( *a )[d[ec.children[i]].getCacheData().position],ec.weights[i])) ;
			} catch (const PaloException& ) {
			}
		}
		dei.num_children = dei.children.size();

		dei.parents.reserve( ec.number_parents );
		for ( unsigned int i = 0; i < ec.number_parents; i++ ) {
			try {			
				dei.parents.push_back(( *a )[d[ec.parents[i]].getCacheData().position]);
			} catch (const PaloException& ) {				
			}
		}
		dei.num_parents = dei.parents.size();
	}	

	if(filter) 
	{
		const erase_if_id_zero_helper pred;
		a->erase( std::remove_if( a->begin(), a->end(), pred),a->end() );
	}
	return a;
}


DimensionElementInfoSimpleArray SpreadsheetFuncsBase::FPaloDimensionListElements( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, bool filter ) {
	DimensionElementInfoSimpleArray sa;

	Dimension d = ( *s )[database].dimension[dimension];
	DIMENSION_INFO di = d.getCacheData();

	sa.resize( di.number_elements );

	for ( std::unique_ptr<DimensionCache::CacheIterator> i = d.getIterator(); !( *i ).end(); ( *i )++ ) {
		sa[( *i )->position].name = ( *i )->nelement;
		sa[( *i )->position].type = DimensionElementType(( *i )->type );
		sa[( *i )->position].identifier = ( *i )->element;
	}

	if(filter) 
	{
		const erase_if_id_zero_helper pred;
		sa.erase( std::remove_if( sa.begin(), sa.end(), pred),sa.end() );
	}

	return sa;
}

StringArray SpreadsheetFuncsBase::FPaloDimensionListElementsSimple( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	StringArray sa;

	Dimension d = ( *s )[database].dimension[dimension];
	const DIMENSION_INFO& di = d.getCacheData();

	sa.resize( di.number_elements );

	for ( std::unique_ptr<DimensionCache::CacheIterator> i = d.getIterator(); !( *i ).end(); ( *i )++ )
		sa[( *i )->position] = ( *i )->nelement;

	return sa;
}


StringArray SpreadsheetFuncsBase::FPaloDimensionListCubes( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	StringArray sa;

	Database db = ( *s )[database];
	Dimension d = db.dimension[dimension];
	const DIMENSION_INFO& di = d.getCacheData();

	for ( std::unique_ptr<CubesCache::CacheIterator> i = db.cube.getIterator(); !( *i ).end(); ( *i )++ ) {
		const CubeCache& cc = **i;
		for ( unsigned int j = 0; j < cc.number_dimensions; j++ ) {
			if ( cc.dimensions[j] == di.dimension )
				sa.push_back( cc.ncube );
		}
	}

	return sa;
}


StringArray SpreadsheetFuncsBase::FPaloCubeListDimensions( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube ) {
	StringArray sa;

	Database d = ( *s )[database];
	Cube c = d.cube[cube];
	const CUBE_INFO& ci = c.getCacheData();

	sa.reserve( ci.number_dimensions );

	for ( size_t i = 0; i < ci.number_dimensions; i++ ) {
		sa.push_back( d.dimension[ci.dimensions[i]].getCacheData().ndimension );
	}

	return sa;
}


StringArray SpreadsheetFuncsBase::FPaloDatabaseListCubes( boost::shared_ptr<Server> s, const std::string& database ) {
	StringArray sa;

	Database d = ( *s )[database];
	const DATABASE_INFO& di = d.getCacheData();

	sa.reserve( di.number_cubes );

	for ( std::unique_ptr<CubesCache::CacheIterator> i = d.cube.getIterator(); !( *i ).end(); ( *i )++ ) {
		sa.push_back(( *i )->ncube );
	}

	return sa;
}


StringArray SpreadsheetFuncsBase::FPaloDatabaseListCubes( boost::shared_ptr<Server> s, const std::string& database, CUBE_INFO::TYPE type ) {
	StringArray sa;

	Database d = ( *s )[database];
	const DATABASE_INFO& di = d.getCacheData();

	sa.reserve( di.number_cubes );
	bool doadd;

	for ( std::unique_ptr<CubesCache::CacheIterator> i = d.cube.getIterator(); !( *i ).end(); ( *i )++ ) {
		if (type == CUBE_INFO::SYSTEM) {
			doadd = ((( *i )->type == type ) || ((( *i )->type == CUBE_INFO::ATTRIBUTE ) && isPropertyCube(( *i )->ncube)));
		} else {
			if (type == CUBE_INFO::ATTRIBUTE ) {
				doadd = (( *i )->type == type ) && !isPropertyCube(( *i )->ncube);
			} else {
				doadd = (( *i )->type == type );
			}
		}
		if (doadd) {
			sa.push_back(( *i )->ncube );
		}
	}

	return sa;
}


StringArray SpreadsheetFuncsBase::FPaloDatabaseListDimensions( boost::shared_ptr<Server> s, const std::string& database ) {
	StringArray sa;

	Database d = ( *s )[database];
	const DATABASE_INFO& di = d.getCacheData();

	sa.reserve( di.number_dimensions );

	for ( std::unique_ptr<DimensionsCache::CacheIterator> i = d.dimension.getIterator(); !( *i ).end(); ( *i )++ ) {
		sa.push_back(( *i )->ndimension );
	}

	return sa;
}


StringArray SpreadsheetFuncsBase::FPaloDatabaseListDimensions( boost::shared_ptr<Server> s, const std::string& database, DIMENSION_INFO::TYPE type ) {
	StringArray sa;

	Database d = ( *s )[database];
	const DATABASE_INFO& di = d.getCacheData();

	sa.reserve( di.number_dimensions );

	for ( std::unique_ptr<DimensionsCache::CacheIterator> i = d.dimension.getIterator(); !( *i ).end(); ( *i )++ )
		if (( *i )->type == type )
			sa.push_back(( *i )->ndimension );

	return sa;
}


StringArray SpreadsheetFuncsBase::FPaloRootListDatabases( boost::shared_ptr<Server> s, bool list_system_db, bool list_user_info_db ) {
	StringArray sa;

	for ( std::unique_ptr<ServerCache::CacheIterator> i = s->getIterator(); !( *i ).end(); ( *i )++ ) {
		
		if ( (( *i )->type == DATABASE_INFO::NORMAL ) ) {
			sa.push_back(( *i )->ndatabase );
		} else if( list_user_info_db && (*i )->type == DATABASE_INFO::USERINFO  ) {
			sa.push_back(( *i )->ndatabase );
		} else if ( list_system_db && (( *i )->type == DATABASE_INFO::SYSTEM ) ) {			
			sa.push_back(( *i )->ndatabase );
		}		
	}

	return sa;
}


void SpreadsheetFuncsBase::FPaloDatabaseRenameDimension( boost::shared_ptr<Server> s, const std::string& database, const std::string& old_name, const std::string& new_name ) {
	( *s )[database].dimension[old_name].rename( new_name );
}


CellValue SpreadsheetFuncsBase::FPaloSetdata( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const StringArray& path, const CellValue& value, SPLASH_MODE splash ) {
	long dbid = ( *s )[database].getCacheData().database;
	long cid = ( *s )[dbid].cube[cube].getCacheData().cube;

	if ( value.type == CellValue::NUMERIC ) {
		( *s )[dbid].cube[cid].CellReplace( path, value.toPalo(), splash );
		return value;
	} else if ( value.type == CellValue::STRING ) {
		StringArrayArray _temp;
		_temp.resize(path.size());
		for(StringArray::const_iterator i = path.begin(); i != path.end(); ++i) {
			_temp[i-path.begin()].push_back(*i);
		}
		std::vector<CELL_VALUE_EXPORTED> _vals = ( *s )[dbid].cube[cid].CellExport( _temp, 2, StringArray(), "", 0, 0 );
		if(_vals.size() < 2) {
			throw SpreadsheetFuncsException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_FUNCTION_CALL_FAILED );
		}
		CellValue cur = _vals[0].cvp;

		if ( cur.type == CellValue::STRING ) {
			( *s )[dbid].cube[cid].CellReplace( path, value.toPalo(), MODE_SPLASH_NONE );
			return value;
		} else if ( cur.type == CellValue::NUMERIC ) {
			// value.type is CellValue::STRING)
			StringArray tokens;
			try {
				tokens = Util::StringTokenizer( value.val.s, ' ', true ).getTokens();
			} catch (const Util::StringTokenizer::StringTokenizerException ) {}

			/* whitespace skipping */
			for( StringArray::iterator i = tokens.begin(); i != tokens.end();) {
				if(i->empty()) {
					i = tokens.erase( i );
				} else {
					if(i->size() > 0 && (*i)[i->size()-1] == ';' && i+1 != tokens.end()) {
						*i += *(i+1);
						i = tokens.erase(i+1);
					} else {				
						++i;
					}
				}
			}

			if(tokens.empty()) {
				throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_FORMAT );
			}

			const string& val = tokens[0];
			size_t tsize = tokens.size();

			if ( (tsize == 1) && (val.length() > 2) && ( (val[0] == '#') && (val[1] == '#') ) ) {
				// splashing with double hash "##..."
				double scaled_val = cur.val.d + _string2Double( value.val.s.substr( 2 ) );
				( *s )[dbid].cube[cid].CellReplace( path, _double2CELL_VALUE( scaled_val ), MODE_SPLASH_DEFAULT );
			} else if ( (tsize == 1) && (val.length() > 1) && ( (val[0] == '|') || ( (val[0] == '#') && (val[val.length()-1] != '%') ) ) ) {
				// default splashing "|..." or "#..."
				( *s )[dbid].cube[cid].CellReplace( path, _double2CELL_VALUE( _string2Double( value.val.s.substr( 1 ) ) ), MODE_SPLASH_DEFAULT );
			} else if ( (tsize == 1) && (val.length() > 1) && ( (val[0] == '!') && (val[1] != '!') ) ) {
				// base set "!..."
				( *s )[dbid].cube[cid].CellReplace( path, _double2CELL_VALUE( _string2Double( value.val.s.substr( 1 ) ) ), MODE_SPLASH_SET );
			} else if ( (tsize == 1) && (val.length() > 2) && ( (val[0] == '!') && (val[1] == '!') ) ) {
				// base add "!!..."
				( *s )[dbid].cube[cid].CellReplace( path, _double2CELL_VALUE( _string2Double( value.val.s.substr( 2 ) ) ), MODE_SPLASH_ADD );
			} else if ( (tsize == 1) && (val.length() > 2) && ( (val[0] == '#') && (val[val.length()-1] == '%') ) ) {
				// splashing with percentage
				double scaled_val = cur.val.d * (( 100.0 + _string2Double( value.val.s.substr( 1, val.length() - 2 ) ) ) / 100.0 );
				( *s )[dbid].cube[cid].CellReplace( path, _double2CELL_VALUE( scaled_val ), MODE_SPLASH_DEFAULT );
			} else  {
				// copy / like
				if (( (tsize == 3) && (StringUtils::toUpper( tokens[1] ) == "LIKE") )
				        || ( (tsize == 2) && (StringUtils::toUpper( tokens[0] ) == "COPY") ) ) {
					double d;
					string coord_str;
					bool is_like, is_percentage = false;
					if ( tsize == 3 ) {
						if ( tokens[0].c_str()[0] == '#' ) {
							d = _string2Double( tokens[0].substr( 1 ), is_percentage );
							if ( !is_percentage )
								throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_FORMAT );
						} else {
							d = _string2Double( tokens[0], is_percentage );
							if ( is_percentage )
								throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_FORMAT );
						}
						is_like = true;
						coord_str = tokens[2];
					} else {
						is_like = false;
						coord_str = tokens[1];
					}

					StringArray coords;
					try {
						coords = Util::StringTokenizer( coord_str, ';', true ).getTokens();
					} catch (const Util::StringTokenizer::StringTokenizerException ) {
						throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_FORMAT );
					}
					StringArray realCoords = path;
					StringArray cubeDims = FPaloCubeListDimensions( s, database, cube );
					for ( size_t i = 0; i < coords.size(); i++ ) {
						StringArray t;
						try {
							t = Util::StringTokenizer( coords[i], ':', true ).getTokens();
						} catch (const Util::StringTokenizer::StringTokenizerException ) {
							throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_FORMAT );
						}
						if ( t.size() == 1 ) {
							bool found = false;
							size_t pos;
							size_t number_dimensions = ( *s )[dbid].cube[cid].getCacheData().number_dimensions;
							for ( size_t i = 0; i < number_dimensions; i++ ) {
								try {
									( *s )[dbid].dimension[( *s )[dbid].cube[cid].getCacheData().dimensions[i]][t[0]];
									// name not unique
									if ( found ){
										throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_NAME_NOT_UNIQUE );
									}

									found = true;
									pos = i;
								} catch (const ElementNotFoundException ) {}
							}
							if ( !found ) {
								throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_NOT_FOUND );
							}
							realCoords[pos] = t[0];
						} else if ( t.size() == 2 ) {
							StringArray::const_iterator i;
							for ( i = cubeDims.begin(); i != cubeDims.end(); i++ ) {
								if ( StringUtils::toUpper( *i ) == StringUtils::toUpper( t[0] ) ) {
									break;
								}
							}
							if ( i == cubeDims.end() ) {
								throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_NOT_FOUND );
							}
							realCoords[i - cubeDims.begin()] = t[1];
						} else {
							throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_FORMAT );
						}
					}

					if ( is_like ) {
						if ( is_percentage ) {
							CellValue cv = ( *s )[dbid].cube[cid].CellValue( realCoords );

							if ( cv.type == CellValue::NUMERIC ) {
								d = ( 100.0 + d ) * cv.val.d / 100.0;
							} else {
								throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_DIMENSION_ELEMENT_TYPE );
							}
						}

						( *s )[dbid].cube[cid].CellCopy( realCoords, path, d );
					} else {
						( *s )[dbid].cube[cid].CellCopy( realCoords, path );
					}
				} else {
					( *s )[dbid].cube[cid].CellReplace( path, _double2CELL_VALUE( _string2Double( value.val.s ) ), splash );
				}
			}

			return ( *s )[dbid].cube[cid].CellValue( path );
		} else {
			throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_TYPE );
		}
	} else {
		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_TYPE );
	}
}

bool SpreadsheetFuncsBase::FPaloCellCopy( boost::shared_ptr<jedox::palo::Server> s, const std::string& database, const std::string& cube, const StringArray& from, const StringArray& to, const CellValue& value, bool use_rules ) {

	if ( value.type == CellValue::ERR ) {
		return ( *s )[database].cube[cube].CellCopy( from, to, use_rules );
	} else {
		return ( *s )[database].cube[cube].CellCopy( from, to, value.toPalo().val.d, use_rules );
	}
}

bool SpreadsheetFuncsBase::FPaloGoalSeek( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const StringArray& path, const double value, GoalSeekType type, StringArrayArray area)  {
	return ( *s )[database].cube[cube].CellGoalSeek( path, value, (jedox::palo::Cube::GOALSEEK_TYPE)type, area );
}

CellValue SpreadsheetFuncsBase::FPaloGetdata( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const StringArray& path ) {
	return CellValue(( *s )[database].cube[cube].CellValue( path, 0, 1 ));
}

CellValueWithProperties SpreadsheetFuncsBase::FPaloGetdataAggregation( boost::shared_ptr<jedox::palo::Server> s, const std::string& database, const std::string& cube, const StringArray& path, AggregationType aggregationtype, IntArray expandtypes ) {
	size_t i, size1 = path.size(), size2 = expandtypes.size();
	size_t minsize = (size1 < size2) ? size1 : size2;
	vector<CELL_VALUE_PATH_PROPS> area_res;
	vector<vector<string> > area(size1);
	vector<Cube::EXPANDAGGR_TYPE> expand_types(size1, (jedox::palo::Cube::EXPANDAGGR_TYPE)ExpandSelf);

	for (i = 0; i < size1; i++) {
		area[i].push_back(path[i]);
	}

 	for (i = 0; i < minsize; i++) {
		expand_types[i] = (jedox::palo::Cube::EXPANDAGGR_TYPE)expandtypes[i];
	}
	 
	( *s )[database].cube[cube].CellAreaPropsAggr(area_res, area, PropertyNames::PropNames, (jedox::palo::Cube::AGGREGATION_TYPE)aggregationtype, expand_types);

	if (area_res.size() == 0) {
		throw SpreadsheetFuncsException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_FUNCTION_CALL_FAILED );
	}

	 return area_res[0];
}

template <class ServerType> 
CellValue SpreadsheetFuncsBase::FPaloGetdataC( ServerType s, const std::string& database, const std::string& cube, const StringArray& path, const CellValue* *const ptr ) {
	switch ( qc.getStatus() ) {
		case QueryCache::Collect:
			qc.addRequest( s, database, cube, path, ptr );
			return CellValue( ErrorInfo(XLError::NAxl, 0, strNoValueYet) );
		case QueryCache::Return:
			return qc.getResult( s, database, cube, path );
		default:
			// should never happen
			throw InvalidRequestException( CurrentSourceLocation );
	}
}

CellValue SpreadsheetFuncsBase::FPaloGetdataC( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const StringArray& path, const CellValue* *const ptr ) {
	return FPaloGetdataC<boost::shared_ptr<Server> >( s, database, cube, path, ptr );
}

CellValue SpreadsheetFuncsBase::FPaloGetdataC( const std::string& s, const std::string& database, const std::string& cube, const StringArray& path, const CellValue* *const ptr ) {
	return FPaloGetdataC<const std::string&>( s, database, cube, path, ptr );
}

CellValueArray SpreadsheetFuncsBase::FPaloGetdataV( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const StringArrayArray& area ) {
	vector<CELL_VALUE_PATH> a = ( *s )[database].cube[cube].CellArea( area , 0, 1);
	CellValueArray cva;

	cva.reserve( a.size() );

	for ( vector<CELL_VALUE_PATH>::const_iterator i = a.begin(); i != a.end(); i++ ) {
		cva.push_back( CellValue( *i ) );
	}

	return cva;
}

void SpreadsheetFuncsBase::FPaloElementCreateBulk( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const StringArray &elements, DimensionElementType type, const StringArrayArray children, const StringArrayArray weights ) {
	Dimension d = ( *s )[database].dimension[dimension];	
	std::vector<std::vector<double> > weight;
	for(size_t i=0;i<weights.size();i++) {
		std::vector<double> _tmp;
		for(size_t j=0;j<weights[i].size();j++) {			
			_tmp.push_back(boost::lexical_cast<double>(weights[i][j]));
		}
		weight.push_back(_tmp);
	}
	d.bulkCreateElements(elements, type.toPalo(), children, weight);
}


string SpreadsheetFuncsBase::FPaloGetUserForSID( boost::shared_ptr<Server> s, const std::string& sid ) {
	return s->getUserInfo(sid).nuser;
}


StringArray  SpreadsheetFuncsBase::FPaloGetGroupsForSID( boost::shared_ptr<Server> s, const std::string& sid ) {
	return s->getUserInfo(sid).ngroups;
}


StringArray  SpreadsheetFuncsBase::FPaloGetGroups( boost::shared_ptr<Server> s ) {
	//make sure, sid is up to date
	try {
		s->getUserInfo(emptystring);
	} catch (...) {
	}

	return s->getUserInfo(s->getSID()).ngroups;
}


string SpreadsheetFuncsBase::FPaloElementAdd( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, DimensionElementType type, const std::string& element, const std::string& parent, double cfactor, int clear ) {
	Dimension d = ( *s )[database].dimension[dimension];
	vector<string> children;
	children.push_back( element );
	vector<double> weights;
	weights.push_back( cfactor );

	if ( clear == 1 ){ 
		if ( clearedDimensions.find( UniqueDimensionIdx( s, database, dimension ) ) == clearedDimensions.end() ) {
			FPaloDimensionClear( s, database, dimension );
			clearedDimensions[UniqueDimensionIdx( s, database, dimension )] = true;
		}
	} else if ( clear == 2) {
		if ( clearedDimensions.find( UniqueDimensionIdx( s, database, dimension ) ) == clearedDimensions.end() ) {
			Dimension d = ( *s )[database].dimension[dimension];
			std::vector<std::string> elements;
			for ( std::unique_ptr<DimensionCache::CacheIterator> i = d.getIterator(); !( *i ).end(); ( *i )++ ) {
				if (( *i )->type == ELEMENT_INFO::CONSOLIDATED ) {				
					elements.push_back( (**i).nelement );
				}		
			}
			( *s )[database].dimension[dimension].bulkDeleteElements(elements);
			clearedDimensions[UniqueDimensionIdx( s, database, dimension )] = true;
		}
	}

	if ( !parent.empty() ) {
		if(!d.Exists(parent)) {
			d.createElement( parent, ELEMENT_INFO::CONSOLIDATED, vector<string>(), vector<double>() );
		}
	}

	if( !d.Exists(element) ) {
		d.createElement( element, type.toPalo(), vector<string>(), vector<double>() );
	}

	if ( !parent.empty() ) {
		d[parent].append( children, weights );
	}

	return element;
}


bool SpreadsheetFuncsBase::FPaloElementDelete( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return ( *s )[database].dimension[dimension][element].destroy();
}

void SpreadsheetFuncsBase::FPaloElementDeleteBulk( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, std::vector<std::string> elements ) {	
	( *s )[database].dimension[dimension].bulkDeleteElements( elements );		
}

void SpreadsheetFuncsBase::FPaloElementMove( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element, unsigned int new_offset ) {
	( *s )[database].dimension[dimension][element].move( new_offset );
}


void SpreadsheetFuncsBase::FPaloElementRename( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& old_name, const std::string& new_name ) {
	( *s )[database].dimension[dimension][old_name].rename( new_name );
}


void SpreadsheetFuncsBase::FPaloElementUpdate( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element, const DimensionElementType& type, const ConsolidationElementArray& children, bool append ) {
	vector<string> vchildren;
	vector<double> vweights;

	if ( type == DimensionElementType::Consolidated ) {
		for ( ConsolidationElementArray::const_iterator i = children.begin(); i != children.end(); i++ ) {
			vchildren.push_back( i->name );
			vweights.push_back( i->weight );
		}
	}

	if (append) {
		( *s )[database].dimension[dimension][element].append(vchildren, vweights );
	} else {
		( *s )[database].dimension[dimension][element].replace( type.toPalo(), vchildren, vweights );
	}
	s->ping();
}

void SpreadsheetFuncsBase::FPaloChildrenDelete( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element, const StringArray& children ) {
	ELEMENT_INFO ei = ( *s )[database].dimension[dimension][element].getCacheData();

	size_t i, j, vsize = children.size(),vsize2 = ei.number_children - vsize;

	vector<string> vchildren;
	vector<double> vweights;
	vector<string> Children(vsize);

	vchildren.reserve(vsize2);
	vweights.reserve(vsize2);

	for ( i = 0; i < vsize; i++ ) {
		Children[i] = StringUtils::toUpper(children[i]);
	}

	ELEMENT_INFO cei;
	std::string cmpstr;

	for ( i = 0; i < ei.number_children; i++ ) {
		cei = ( *s )[database].dimension[dimension][ei.children[i]].getCacheData();
		cmpstr = StringUtils::toUpper(cei.nelement);

		j = 0;
		while ( (j < vsize) && (cmpstr != Children[j])) {
			j++;
		}
		
		if (j == vsize) {
			vchildren.push_back( cei.nelement );
			vweights.push_back( ei.weights[i] );
		}
	}

	( *s )[database].dimension[dimension][element].replace( ei.type, vchildren, vweights );
}

unsigned int SpreadsheetFuncsBase::FPaloElementChildcount( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return ( *s )[database].dimension[dimension][element].getCacheData().number_children;
}


const std::string& SpreadsheetFuncsBase::FPaloElementChildname( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element, unsigned int child_offset ) {
	Dimension d = ( *s )[database].dimension[dimension];
	const ELEMENT_INFO& ei = d[element].getCacheData();

	if ( ei.number_children < child_offset  || child_offset == 0 )
		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_OFFSET );

	return d[ei.children[child_offset-1]].getCacheData().nelement;
}


unsigned int SpreadsheetFuncsBase::FPaloElementCount( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	return ( *s )[database].dimension[dimension].getCacheData().number_elements;
}


const std::string& SpreadsheetFuncsBase::FPaloElementFirst( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	return PaloHelper::findDimensionElementAt(( *s )[database].dimension[dimension], 0, true ).nelement;
}


unsigned int SpreadsheetFuncsBase::FPaloElementIndex( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return ( *s )[database].dimension[dimension][element].getCacheData().position + backwards_startindex;
}


bool SpreadsheetFuncsBase::FPaloElementIsChild( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& parent, const std::string& child ) {
	Dimension d = ( *s )[database].dimension[dimension];

	long parent_id = d[parent].getCacheData().element;

	const ELEMENT_INFO child_ei = d[child].getCacheData();

	for ( unsigned int i = 0; i < child_ei.number_parents; i++ )
		if ( child_ei.parents[i] == parent_id )
			return true;

	return false;
}


unsigned int SpreadsheetFuncsBase::FPaloElementLevel( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return ( *s )[database].dimension[dimension][element].getCacheData().level;
}


unsigned int SpreadsheetFuncsBase::FPaloElementIndent( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return ( *s )[database].dimension[dimension][element].getCacheData().indent;
}


const std::string& SpreadsheetFuncsBase::FPaloElementName( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, unsigned int offset ) {
	if ( offset < backwards_startindex ) {
		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_OFFSET );
	} else {
		return PaloHelper::findDimensionElementAt(( *s )[database].dimension[dimension], ( offset - backwards_startindex ) ).nelement;
	}
}


const std::string SpreadsheetFuncsBase::FPaloElementName( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return element;
}


CellValue SpreadsheetFuncsBase::FPaloElementName( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element, const std::string& attribute ) {
	Database db = ( *s )[database];
	Dimension d = db.dimension[dimension];
	Cube ac = db.cube[d.getCacheData().attribute_cube];

	StringArray coords;
	coords.reserve( 2 );
	coords.push_back( attribute );
	coords.push_back( element );
	CellValue cv = ac.CellValue( coords );

	if ( cv.type == CellValue::STRING && cv.val.s.empty() ) {
		return CellValue( element );
	} else {
		return cv;
	}
}


unsigned int SpreadsheetFuncsBase::FPaloElementParentcount( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return ( *s )[database].dimension[dimension][element].getCacheData().number_parents;
}


const std::string& SpreadsheetFuncsBase::FPaloElementParentname( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element, unsigned int offset ) {
	Dimension d = ( *s )[database].dimension[dimension];

	const ELEMENT_INFO& ei = d[element].getCacheData();
	if ( ei.number_parents < offset || offset == 0 )
		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_OFFSET );

	return d[ei.parents[offset-1]].getCacheData().nelement;
}


const std::string& SpreadsheetFuncsBase::FPaloElementPrev( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	Dimension d = ( *s )[database].dimension[dimension];

	unsigned int pos = d[element].getCacheData().position;
	if ( pos == 0 ) {
		throw SpreadsheetFuncsException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_IS_FIRST );
	} else {
		return PaloHelper::findDimensionElementAt( d, pos - 1, false ).nelement;
	}
}


const std::string& SpreadsheetFuncsBase::FPaloElementNext( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	Dimension d = ( *s )[database].dimension[dimension];

	return PaloHelper::findDimensionElementAt( d, d[element].getCacheData().position + 1, true ).nelement;
}


const std::string& SpreadsheetFuncsBase::FPaloElementSibling( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element, int sibling_offset ) {
	using namespace jedox::palo;

	Dimension d = ( *s )[database].dimension[dimension];

	const ELEMENT_INFO& ei = d[element].getCacheData();

	if ( ei.number_parents == 0 ) {
		// root element

		if (( sibling_offset > 0 && ei.position + sibling_offset >= d.getCacheData().number_elements )
		        || ( sibling_offset < 0 && ( signed )ei.position < -sibling_offset ) )
			throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_OFFSET );

		return PaloHelper::findDimensionElementAt( d, ei.position + sibling_offset ).nelement;
	} else {
		for ( unsigned int i = 0; i < ei.number_parents; i++ ) {
			const ELEMENT_INFO& parent = d[ei.parents[i]].getCacheData();
			unsigned int pos;

			for ( unsigned int j = 0; j < parent.number_children; j++ ) {
				if ( parent.children[j] == ei.element ) {
					pos = j;
					break;
				}

				if ( j == parent.number_children - 1 )
					throw InvalidRequestException( CurrentSourceLocation );
			}

			if (( sibling_offset > 0 && pos + sibling_offset >= parent.number_children )
			        || ( sibling_offset < 0 && ( signed )pos < -sibling_offset ) ) {
				continue;
			} else {
				return d[parent.children[pos + sibling_offset]].getCacheData().nelement;
			}
		}

		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_OFFSET );
	}
}


unsigned int SpreadsheetFuncsBase::FPaloDimensionMaxLevel( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension ) {
	return ( *s )[database].dimension[dimension].getCacheData().maximum_level;
}


DimensionElementType SpreadsheetFuncsBase::FPaloElementType( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element ) {
	return DimensionElementType(( *s )[database].dimension[dimension][element].getCacheData().type );
}


double SpreadsheetFuncsBase::FPaloElementWeight( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& parent, const std::string& child ) {
	using namespace jedox::palo;

	Dimension d = ( *s )[database].dimension[dimension];

	const ELEMENT_INFO& ei_child = d[child].getCacheData();
	const ELEMENT_INFO& ei_parent = d[parent].getCacheData();


	for ( size_t i = 0; i < ei_parent.number_children; i++ ) {
		if ( ei_parent.children[i] == ei_child.element ) {
			return ei_parent.weights[i];
		}
	}

	throw InvalidRequestException( CurrentSourceLocation );
}

boost::shared_ptr<Server> SpreadsheetFuncsBase::FPaloWebInit( const std::string& hostname, unsigned short int port, const std::string& username, const std::string& password ) {
	string key;
    // JMM 09-20-13
	//return boost::shared_ptr<Server>(ServerPool::getInstance().getServer(hostname, port, username, password, key));
    return FPaloInit(hostname, port, username, password, false);
}


boost::shared_ptr<Server> SpreadsheetFuncsBase::FPaloInit( const std::string& hostname, unsigned short int port, const std::string& username, const std::string& password, bool forcecheck ) {
	string key(hostname);

	key += doublepoint;
	key += boost::lexical_cast<std::string>(port);
	key += atsign;
	key += username;
	key += doublepoint;
	key += password;

	if (forcecheck) {
		SpreadsheetFuncsBase::FPaloRemoveConnection(key, false);
	}

	boost::shared_ptr<Server> s = cp.get(SpreadsheetFuncsBase::FPaloRegisterServer( key, hostname, port, username,  password, false ));
	
	/*
	if (forcecheck) {
		s->checkPassword(password);
	} 
	*/

	return s;
}

void SpreadsheetFuncsBase::FPaloEventLockBegin( boost::shared_ptr<Server> s, const std::string& source, const std::string& areaid ) {
	s->EventLockBegin( source, areaid );
}


void SpreadsheetFuncsBase::FPaloEventLockEnd( boost::shared_ptr<Server> s ) {
	s->EventLockEnd();
}


void SpreadsheetFuncsBase::FPaloStartCacheCollect() {
	qc.setStatus( QueryCache::Collect );
}


bool SpreadsheetFuncsBase::FPaloEndCacheCollect(bool check_locks, bool remove_obsolete) {
	if (remove_obsolete) {
		qc.removeObsolete();
	}
	return qc.setStatus( QueryCache::Return, check_locks );
}


bool SpreadsheetFuncsBase::FPaloCacheClearObsolete() {
	return qc.removeObsolete();
}


std::string SpreadsheetFuncsBase::FPaloRegisterServerHelper( const std::string& key, ConnectionPool::ConnectionData &cd, bool do_connect ) {
	cp.add( key, cd );
	if (do_connect) {
		cp.get( cd );
	}
	return key;
}


std::string SpreadsheetFuncsBase::FPaloRegisterServer( const std::string& key, const std::string& hostname, unsigned short int port, const std::string& username, const std::string& password, bool do_connect ) {
	ConnectionPool::ConnectionData cd(hostname, port, username, password);
	return FPaloRegisterServerHelper(key, cd, do_connect);
}


std::string SpreadsheetFuncsBase::FPaloRegisterServer( const std::string& key, const std::string& hostname, unsigned short int port, const std::string& sid, bool do_connect  ) {
	ConnectionPool::ConnectionData cd(hostname, port, sid);
	return FPaloRegisterServerHelper(key, cd, do_connect);
}

void SpreadsheetFuncsBase::FPaloRemoveConnection( const std::string& key, const bool do_logout ) {
	cp.remove(key, do_logout);
}

bool SpreadsheetFuncsBase::FPaloPing( boost::shared_ptr<Server> s ) {
	return s->ping();
}

unsigned int SpreadsheetFuncsBase::FPaloServerToken (  boost::shared_ptr<jedox::palo::Server> s ) {
	return s->getServerToken();
}


ServerInfo SpreadsheetFuncsBase::FPaloServerInfo( boost::shared_ptr<Server> s ) {
	return s->getCacheData();
}

LicenseInfo SpreadsheetFuncsBase::FPaloLicenseInfo( boost::shared_ptr<Server> s ) {
	LicenseInfo li;
	
	try {
        // JMM 09-20-13
        //LICENSE_LIST ll = s->getLicenseInfo();
		//li = ll.licenses[0];        
	} catch ( const PaloServerException& exp ) {
		if (exp.code() != PaloExceptionFactory::ERROR_API_CALL_NOT_IMPLEMENTED) {
			throw;
		}
	}

	return li;
}

void SpreadsheetFuncsBase::FPaloChangePassword( boost::shared_ptr<Server> s, const std::string& oldpassword, const std::string& newpassword ) {
	s->changePassword(oldpassword, newpassword);
}


const std::string SpreadsheetFuncsBase::_validate_bool_op( const std::string& compare_mode ) {
	if ( (StringUtils::toUpper( compare_mode ) == "AND") || (StringUtils::toUpper( compare_mode ) == "OR") || (StringUtils::toUpper( compare_mode ) == "XOR") ) {
		return StringUtils::toUpper( compare_mode );
	} else {
		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_BOOL_OP );
	}
}


const std::string SpreadsheetFuncsBase::_compare_op2encString( const std::string& compare_op ) {
	if ( compare_op == "lt" )
		return "<";
	else if ( compare_op == "gt" )
		return ">";
	else if ( compare_op == "lte" )
		return "<=";
	else if ( compare_op == "gte" )
		return ">=";
	else if ( compare_op == "eq" )
		return "==";
	else if ( compare_op == "neq" )
		return "!=";
	else
		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_COMPARE_OP );
}


const std::string SpreadsheetFuncsBase::_CellValue2encString( const CellValue& cv ) {
	switch ( cv.type ) {
		case CellValue::STRING:
			return jedox::util::StringUtils::URLencode( jedox::util::StringUtils::CSVencode( cv.val.s ) );
		case CellValue::NUMERIC: {
			ostringstream o;
			o << cv.val.d;
			return o.str();
		}
		default:
			throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_TYPE );
	}
}


SpreadsheetFuncsBase::GetdataExportResult SpreadsheetFuncsBase::FPaloGetdataExport( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, bool ignore_empty, bool base_only, const std::string& condition, unsigned int max_rows, const StringArray& first_path, const ElementListArray& area, unsigned short use_rules ) {
	Database db = ( *s )[database];
	Cube c = db.cube[cube];

	vector<CELL_VALUE_EXPORTED> v = c.CellExport( area.getStringArrayArray(), max_rows, first_path, jedox::util::StringUtils::URLencode( condition ), base_only, ignore_empty, use_rules );
	// last entry indicates progress

	if ( v.size() < 1 ) {
		throw InvalidRequestException( CurrentSourceLocation );
	} else {
		v.resize( v.size() - 1 );
	}

	GetdataExportResult ger;

	for ( vector<CELL_VALUE_EXPORTED>::const_iterator i = v.begin(); i != v.end(); i++ ) {
		ger.push_back( GetdataExportResultRow( db, c, *i ) );
	}

	return ger;
}


SpreadsheetFuncsBase::GetdataExportResult SpreadsheetFuncsBase::FPaloGetdataExport( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, bool ignore_empty, bool base_only, const std::string& condition, unsigned int max_rows, const StringArray& first_path, unsigned short use_rules ) {
	ElementListArray ela;

	const CUBE_INFO& ci = ( *s )[database].cube[cube].getCacheData();

	ela.resize( ci.number_dimensions );

	for ( size_t i = 0; i < ci.number_dimensions; i++ ) {
		ela[i].setAll();
	}

	return FPaloGetdataExport( s, database, cube, ignore_empty, base_only, condition, max_rows, first_path, ela, use_rules );
}


SpreadsheetFuncsBase::GetdataExportResult SpreadsheetFuncsBase::FPaloGetdataExport( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, bool ignore_empty, bool base_only, const CellValue& lower_limit, const CellValue& upper_limit, const std::string& lower_operator, const std::string& upper_operator, const std::string& bool_op, unsigned int max_rows, const StringArray& first_path, const ElementListArray& area, unsigned short use_rules ) {
	ostringstream condition;

	condition << _compare_op2encString( lower_operator ) << _CellValue2encString( lower_limit ) << " " << _validate_bool_op( bool_op ) << " " << _compare_op2encString( upper_operator ) << _CellValue2encString( upper_limit );

	return FPaloGetdataExport( s, database, cube, ignore_empty, base_only, condition.str(), max_rows, first_path, area, use_rules );
}


SpreadsheetFuncsBase::GetdataExportResult SpreadsheetFuncsBase::FPaloGetdataExport( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, bool ignore_empty, bool base_only, const CellValue& lower_limit, const CellValue& upper_limit, const std::string& lower_operator, const std::string& upper_operator, const std::string& bool_op, unsigned int max_rows, const StringArray& first_path, unsigned short use_rules ) {
	ElementListArray ela;

	const CUBE_INFO& ci = ( *s )[database].cube[cube].getCacheData();

	ela.resize( ci.number_dimensions );

	for ( size_t i = 0; i < ci.number_dimensions; i++ ) {
		ela[i].setAll();
	}

	return FPaloGetdataExport( s, database, cube, ignore_empty, base_only, lower_limit, upper_limit, lower_operator, upper_operator, bool_op, max_rows, first_path, ela, use_rules );
}


SpreadsheetFuncsBase::GetdataExportResultRow::GetdataExportResultRow( Database& db, Cube& c, const CELL_VALUE_EXPORTED& cve ) {
	for ( ELEMENT_LIST::const_iterator i = cve.cvp.path.begin(); i != cve.cvp.path.end(); i++ ) {
		path.push_back( db.dimension[c.getCacheData().dimensions[i-cve.cvp.path.begin()]][*i].getCacheData().nelement );
	}

	switch ( cve.cvp.type ) {
		case CELL_VALUE::NUMERIC:
			val.type = CellValue::NUMERIC;
			val.val.d = cve.cvp.val.d;
			break;

		case CELL_VALUE::STRING:
			val.type = CellValue::STRING;
			val.val.s = cve.cvp.val.s;
			val.val.d = 0;
			break;

		case CELL_VALUE::ERROR:
			val.type = CellValue::ERR;
			val.val.s = cve.cvp.val.s;
			val.val.d = cve.cvp.val.errorcode;
			val.val.err = ErrorInfo( XLError::libpalo_ng_code2XLError( cve.cvp.val.errorcode ), cve.cvp.val.errorcode, cve.cvp.val.s );
			break;

		default:
			throw SpreadsheetFuncsException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_TYPE );
	}
}


double SpreadsheetFuncsBase::_string2Double( const std::string& s ) {
	bool b;

	return _string2Double( s, b );
}


double SpreadsheetFuncsBase::_string2Double( const std::string& s, bool& is_percentage ) {

	size_t pos = s.find_first_of(',');

	istringstream in( (pos == std::string::npos) ? s : std::string(s).replace(pos,1, ".") );

	double d;

	in >> d;

	char c;
	if ( !in.eof() ) {
		in >> c;
		if ( c == '%' ) {
			is_percentage = true;
		} else {
			throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_VALUE );
		}
	} else {
		is_percentage = false;
	}

	if ( in.fail() || !in.eof() ) {
		throw ArgumentException( CurrentSourceLocation, SpreadsheetFuncsErrors::ERROR_INVALID_ARGUMENT_VALUE );
	}

	return d;
}


CELL_VALUE SpreadsheetFuncsBase::_double2CELL_VALUE( double d ) {
	CELL_VALUE cv;

	cv.type = CELL_VALUE::NUMERIC;
	cv.val.d = d;

	return cv;
}


void SpreadsheetFuncsBase::FPaloAuthSID( const std::string& host, unsigned short port, const std::string& sid ) {
	FPaloRegisterServer( "PushRuleEventSource", host, port, sid );
}


RuleInfo SpreadsheetFuncsBase::FPaloCubeRuleCreate( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const std::string& definition, std::string extern_id , std::string comment, bool activate ) {
	RULE_INFO ri = ( *s )[database].cube[cube].RuleCreate( definition, 0, extern_id , comment , activate );

	return RuleInfo( ri );
}


RuleInfo SpreadsheetFuncsBase::FPaloCubeRuleModify( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, long int identifier, const std::string& definition, std::string extern_id , std::string comment, bool activate ) {
	RULE_INFO ri = ( *s )[database].cube[cube].RuleModify( identifier, definition, 0, extern_id , comment , activate );

	return RuleInfo( ri );
}


std::string SpreadsheetFuncsBase::FPaloCubeRuleParse( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const std::string& definition ) {
	return ( *s )[database].cube[cube].RuleParse( definition );
}


RuleInfoArray SpreadsheetFuncsBase::FPaloCubeRules( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube ) {
	return RuleInfoArray(( *s )[database].cube[cube].Rules() );
}


bool SpreadsheetFuncsBase::FPaloCubeRuleDelete( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, long identifier ) {
	return ( *s )[database].cube[cube].RuleDestroy( identifier );
}


bool SpreadsheetFuncsBase::FPaloSetdataBulk( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube, const StringArrayArray& coordinates, const CellValueArray& values, SPLASH_MODE splash ) {
	vector<CELL_VALUE> vals;
	
	vals.reserve( values.size() );
	
	for ( CellValueArray::const_iterator i = values.begin(); i != values.end(); i++ ) {
		vals.push_back( i->toPalo() );
	}
	
	return ( *s )[database].cube[cube].CellReplaceBulk( coordinates, vals, splash );
}


SubsetResults SpreadsheetFuncsBase::FPaloSubset( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension,
                                                 int indent,
                                                 const BasicFilterSettings& basic, const TextFilterSettings& text,
                                                 const SortingFilterSettings& sorting, const AliasFilterSettings& alias,
                                                 const FieldFilterSettings& field,
                                                 const StructuralFilterSettings& structural, const DataFilterSettings& data ) {

	ElementExList el;
	( *s )[database].dimension[dimension].subset(el, s.get(), alias, field, basic, data, sorting, structural, text );

	SubsetResults srs;

	for ( ElementExList::iterator i = el.begin(); i != el.end(); ++i ) {
		srs.push_back( SubsetResult( i->get_name(), i->get_alias(), i->get_idx( indent)));
	}

	return srs;
}

long SpreadsheetFuncsBase::FPaloGetElementId( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& element )
{
	return (*s)[database].dimension[dimension][element].getCacheData().element;
}

const std::string& SpreadsheetFuncsBase::FPaloGetElementName( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, long elementId )
{
	return (*s)[database].dimension[dimension][elementId].getCacheData().nelement;
}

long SpreadsheetFuncsBase::FPaloGetDimensionId( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension )
{
	return (*s)[database].dimension[dimension].getCacheData().dimension;
}

const std::string& SpreadsheetFuncsBase::FPaloGetDimensionName(boost::shared_ptr<Server> s, const std::string& database, long dimensionId)
{
	return (*s)[database].dimension[dimensionId].getCacheData().ndimension;
}

long SpreadsheetFuncsBase::FPaloGetCubeId( boost::shared_ptr<Server> s, const std::string& database, const std::string& cube )
{
	return (*s)[database].cube[cube].getCacheData().cube;
}

const std::string& SpreadsheetFuncsBase::FPaloGetCubeName(boost::shared_ptr<Server> s, const std::string& database, long cubeId)
{
	return (*s)[database].cube[cubeId].getCacheData().ncube;
}

StringArray SpreadsheetFuncsBase::FPaloElementAlias( boost::shared_ptr<Server> s, const std::string& database, const std::string& dimension, const std::string& attribute, const std::string& alias_value, unsigned long idx )
{
	Database db = ( *s )[database];
	Dimension dim = db.dimension[dimension];
	long attcube = dim.getCacheData().attribute_cube;	
	Cube c = db.cube[attcube];
	
	StringArray start;
	StringArrayArray area;
	area.resize(2);
	
	StringArray aa;	
	aa.push_back(attribute);
	area[0] = aa;

	StringArray bb;
	area[1] = bb;

	vector<CELL_VALUE_EXPORTED> v = c.CellExport( area, 10000, start, "", 0, 0, 0);
	size_t size = v.size();
	
	StringArray retval;
	for(size_t i=0;i<size;i++)
	{
		if (v[i].cvp.val.s == alias_value) {
			retval.push_back(dim[v[i].cvp.path[1]].getCacheData().nelement);						
		}
	}	

	if(idx > 0 && idx <= retval.size()) {
		return StringArray(1, retval[idx-1]);		
	}
	return retval;
}