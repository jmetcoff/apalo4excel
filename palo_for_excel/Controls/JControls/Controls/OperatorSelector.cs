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
 * 
 *
 */

////////////////////////////////////////////////////////////////////////////////
/// @brief  OperatorSelector control
///
/// @file   OperatorSelector.cs
///
/// Developed by Igor Timarac, Banja Luka on behalf of Jedox GmbH.
/// Copyright and exclusive worldwide exploitation right has
/// Jedox GmbH, Freiburg.
///
/// @author Igor Timarac, Banja Luka, Bosnia and Herzegovina
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Windows.Forms;

namespace JControls
{
    public partial class OperatorSelector : UserControl
    {
        public OperatorSelector()
        {
            InitializeComponent();
        }

        // WARNING (ContextMenuStrip memory leak):
        public ContextMenuStrip CMSOperator
        {
            get { return cmsOperator; }
        }

        public void DisableRemove()
        {
            this.removeToolStripMenuItem.Enabled = false;
        }

        public void EnableRemove()
        {
            this.removeToolStripMenuItem.Enabled = true;
        }

        public String Operator
        {
            get { return lbOperator.Text; }
            set { 
                lbOperator.Text = value;
                if (value != "")
                {
                    tGrid.CellBorderStyle = TableLayoutPanelCellBorderStyle.Single;
                }
                else
                {
                    tGrid.CellBorderStyle = TableLayoutPanelCellBorderStyle.None;
                }
            }
        }

        public Button BSelect
        {
            get { return bSelect; }
        }

        private void bSelect_Click(object sender, EventArgs e)
        {
            cmsOperator.Show((Control)sender,0,0);
        }
    }
}