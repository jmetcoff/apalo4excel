/* 
 *
 * Copyright (C) 2006-2014 Jedox AG
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (Version 2) as published
 * by the Free Software Foundation at http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * If you are developing and distributing open source applications under the
 * GPL License, then you are free to use Palo under the GPL License.  For OEMs,
 * ISVs, and VARs who distribute Palo with their products, and do not license
 * and distribute their source code under the GPL, Jedox provides a flexible
 * OEM Commercial License.
 *
 * \author Marek Pikulski <marek.pikulski@jedox.com>
 * 
 *
 */

#ifndef PHP_PALO_CONTEXT_H
#define PHP_PALO_CONTEXT_H

#include <PaloSpreadsheetFuncs/GenericContext.h>

namespace Palo {
namespace PHPPalo {
using namespace SpreadsheetFuncs;

class PHPPaloContext : public GenericContext {
public:
	PHPPaloContext();

	bool splashingAllowed();
	void setEncodingConversion(bool new_setting);
	Conversions& getConversions();

private:
	bool encoding_conversion;
	Conversions c;
};
}
}
#endif
