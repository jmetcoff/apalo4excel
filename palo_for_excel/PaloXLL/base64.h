/* 
*
* Copyright (C) 2006-2011 Jedox AG
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
* You may obtain a copy of the License at
*
* <a href="http://www.jedox.com/license_palo_bi_suite.txt">
*   http://www.jedox.com/license_palo_bi_suite.txt
* </a>
*
* If you are developing and distributing open source applications under the
* GPL License, then you are free to use Palo under the GPL License.  For OEMs,
* ISVs, and VARs who distribute Palo with their products, and do not license
* and distribute their source code under the GPL, Jedox provides a flexible
* OEM Commercial License.
*
* \author
* Marek Pikulski <marek.pikulski@jedox.com>
* Dominik Danehl <dominik.danehl@jedox.com>
*
*/


#ifndef HAS_BASE64_H
#define HAS_BASE64_H


#ifdef __cplusplus
extern "C" {

#endif

int base64_encode_get_length( size_t l );

int base64_decode_get_length( size_t l );
	
int base64_encode( const BYTE* pbSrcData, size_t nSrcLen, LPSTR szDest, int* pnDestLen );
	
int base64_decode( LPCSTR szSrc, size_t nSrcLen, BYTE* pbDest, int* pnDestLen );

#ifdef __cplusplus

}
#endif

#endif
