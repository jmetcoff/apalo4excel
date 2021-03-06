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

namespace Apalo.XlAddin.Forms
{
    partial class SubSetEditor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
		

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param subsetName="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SubSetEditor));
            this.tspMain = new System.Windows.Forms.ToolStripContainer();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.statusBar = new System.Windows.Forms.ToolStripStatusLabel();
            this.scVertical1 = new System.Windows.Forms.SplitContainer();
            this.cSubsets = new Apalo.XlAddin.JSubSet.Controls.CStoredSubsets();
            this.lbStoredSubsets = new System.Windows.Forms.Label();
            this.scVertical2 = new System.Windows.Forms.SplitContainer();
            this.scMain = new System.Windows.Forms.SplitContainer();
            this.aSubSet = new Apalo.XlAddin.JSubSet.ASubSet();
            this.btPaste = new System.Windows.Forms.Button();
            this.btCopy = new System.Windows.Forms.Button();
            this.btSave = new System.Windows.Forms.Button();
            this.btClose = new System.Windows.Forms.Button();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.ctrlPreview = new Apalo.XlAddin.JSubSet.CtrlPreview();
            this.pNamedParams = new System.Windows.Forms.Panel();
            this.pInnerNamedParams = new System.Windows.Forms.Panel();
            this.lbNamedParams = new System.Windows.Forms.Label();
            this.lbPreview = new System.Windows.Forms.Label();
            this.tmTimer = new System.Windows.Forms.Timer(this.components);
            this.tTip = new System.Windows.Forms.ToolTip(this.components);
            this.tspMain.BottomToolStripPanel.SuspendLayout();
            this.tspMain.ContentPanel.SuspendLayout();
            this.tspMain.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.scVertical1.Panel1.SuspendLayout();
            this.scVertical1.Panel2.SuspendLayout();
            this.scVertical1.SuspendLayout();
            this.scVertical2.Panel1.SuspendLayout();
            this.scVertical2.Panel2.SuspendLayout();
            this.scVertical2.SuspendLayout();
            this.scMain.Panel1.SuspendLayout();
            this.scMain.Panel2.SuspendLayout();
            this.scMain.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.pNamedParams.SuspendLayout();
            this.SuspendLayout();
            // 
            // tspMain
            // 
            // 
            // tspMain.BottomToolStripPanel
            // 
            this.tspMain.BottomToolStripPanel.Controls.Add(this.statusStrip1);
            // 
            // tspMain.ContentPanel
            // 
            this.tspMain.ContentPanel.Controls.Add(this.scVertical1);
            this.tspMain.ContentPanel.Size = new System.Drawing.Size(995, 631);
            this.tspMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tspMain.LeftToolStripPanelVisible = false;
            this.tspMain.Location = new System.Drawing.Point(0, 0);
            this.tspMain.Name = "tspMain";
            this.tspMain.RightToolStripPanelVisible = false;
            this.tspMain.Size = new System.Drawing.Size(995, 653);
            this.tspMain.TabIndex = 0;
            this.tspMain.Text = "tspMain";
            this.tspMain.TopToolStripPanelVisible = false;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.statusStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusBar});
            this.statusStrip1.Location = new System.Drawing.Point(0, 0);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(995, 22);
            this.statusStrip1.TabIndex = 0;
            // 
            // statusBar
            // 
            this.statusBar.AutoSize = false;
            this.statusBar.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.statusBar.Font = new System.Drawing.Font("Tahoma", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.statusBar.Name = "statusBar";
            this.statusBar.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
            this.statusBar.Size = new System.Drawing.Size(980, 17);
            this.statusBar.Spring = true;
            this.statusBar.Text = "statusBar";
            this.statusBar.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // scVertical1
            // 
            this.scVertical1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scVertical1.Location = new System.Drawing.Point(0, 0);
            this.scVertical1.Name = "scVertical1";
            // 
            // scVertical1.Panel1
            // 
            this.scVertical1.Panel1.Controls.Add(this.cSubsets);
            this.scVertical1.Panel1.Controls.Add(this.lbStoredSubsets);
            // 
            // scVertical1.Panel2
            // 
            this.scVertical1.Panel2.Controls.Add(this.scVertical2);
            this.scVertical1.Size = new System.Drawing.Size(995, 631);
            this.scVertical1.SplitterDistance = 169;
            this.scVertical1.TabIndex = 10;
            // 
            // cSubsets
            // 
            this.cSubsets.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.cSubsets.EditorType = Apalo.XlAddin.Forms.EditorType.GlobalOnly;
            this.cSubsets.LoadedSubsetGroup = null;
            this.cSubsets.LoadedSubsetNode = null;
            this.cSubsets.Location = new System.Drawing.Point(1, 26);
            this.cSubsets.Name = "cSubsets";
            this.cSubsets.Size = new System.Drawing.Size(167, 600);
            this.cSubsets.TabIndex = 11;
            // 
            // lbStoredSubsets
            // 
            this.lbStoredSubsets.AutoSize = true;
            this.lbStoredSubsets.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbStoredSubsets.Location = new System.Drawing.Point(3, 9);
            this.lbStoredSubsets.Name = "lbStoredSubsets";
            this.lbStoredSubsets.Size = new System.Drawing.Size(93, 13);
            this.lbStoredSubsets.TabIndex = 0;
            this.lbStoredSubsets.Tag = "Stored Subsets";
            this.lbStoredSubsets.Text = "Stored Subsets";
            // 
            // scVertical2
            // 
            this.scVertical2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scVertical2.Location = new System.Drawing.Point(0, 0);
            this.scVertical2.Name = "scVertical2";
            // 
            // scVertical2.Panel1
            // 
            this.scVertical2.Panel1.Controls.Add(this.scMain);
            // 
            // scVertical2.Panel2
            // 
            this.scVertical2.Panel2.Controls.Add(this.tableLayoutPanel1);
            this.scVertical2.Panel2.Controls.Add(this.lbPreview);
            this.scVertical2.Panel2.Paint += new System.Windows.Forms.PaintEventHandler(this.scVertical2_Panel2_Paint);
            this.scVertical2.Size = new System.Drawing.Size(822, 631);
            this.scVertical2.SplitterDistance = 576;
            this.scVertical2.TabIndex = 30;
            // 
            // scMain
            // 
            this.scMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.scMain.IsSplitterFixed = true;
            this.scMain.Location = new System.Drawing.Point(0, 0);
            this.scMain.Name = "scMain";
            this.scMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // scMain.Panel1
            // 
            this.scMain.Panel1.Controls.Add(this.aSubSet);
            // 
            // scMain.Panel2
            // 
            this.scMain.Panel2.Controls.Add(this.btPaste);
            this.scMain.Panel2.Controls.Add(this.btCopy);
            this.scMain.Panel2.Controls.Add(this.btSave);
            this.scMain.Panel2.Controls.Add(this.btClose);
            this.scMain.Size = new System.Drawing.Size(576, 631);
            this.scMain.SplitterDistance = 594;
            this.scMain.TabIndex = 100;
            // 
            // aSubSet
            // 
            this.aSubSet.Dimension = "";
            this.aSubSet.Dock = System.Windows.Forms.DockStyle.Fill;
            this.aSubSet.Location = new System.Drawing.Point(0, 0);
            this.aSubSet.Name = "aSubSet";
            this.aSubSet.Size = new System.Drawing.Size(576, 594);
            this.aSubSet.SubsetEditor = null;
            this.aSubSet.TabIndex = 20;
            this.aSubSet.RequestingElements += new System.EventHandler(this.aSubSet_RequestingElements);
            this.aSubSet.QueryXCell += new Apalo.XlAddin.JSubSet.ASubSet.QueryXCellEventHandler(this.aSubSet_QueryXCell);
            // 
            // btPaste
            // 
            this.btPaste.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btPaste.AutoSize = true;
            this.btPaste.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.btPaste.FlatAppearance.BorderSize = 0;
            this.btPaste.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btPaste.Image = ((System.Drawing.Image)(resources.GetObject("btPaste.Image")));
            this.btPaste.ImageAlign = System.Drawing.ContentAlignment.TopLeft;
            this.btPaste.Location = new System.Drawing.Point(411, 4);
            this.btPaste.Name = "btPaste";
            this.btPaste.Size = new System.Drawing.Size(81, 23);
            this.btPaste.TabIndex = 103;
            this.btPaste.Tag = "Paste";
            this.btPaste.Text = "Paste Into";
            this.btPaste.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btPaste.UseVisualStyleBackColor = true;
            this.btPaste.Click += new System.EventHandler(this.pasteSubset_Chosen);
            // 
            // btCopy
            // 
            this.btCopy.FlatAppearance.BorderSize = 0;
            this.btCopy.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btCopy.Image = ((System.Drawing.Image)(resources.GetObject("btCopy.Image")));
            this.btCopy.Location = new System.Drawing.Point(3, 3);
            this.btCopy.Name = "btCopy";
            this.btCopy.Size = new System.Drawing.Size(23, 23);
            this.btCopy.TabIndex = 101;
            this.btCopy.Tag = "Copy";
            this.btCopy.UseVisualStyleBackColor = true;
            this.btCopy.Visible = false;
            this.btCopy.Click += new System.EventHandler(this.copySubset_Chosen);
            // 
            // btSave
            // 
            this.btSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btSave.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btSave.Location = new System.Drawing.Point(330, 4);
            this.btSave.Name = "btSave";
            this.btSave.Size = new System.Drawing.Size(75, 23);
            this.btSave.TabIndex = 102;
            this.btSave.Tag = "Save";
            this.btSave.Text = "Save";
            this.btSave.UseVisualStyleBackColor = true;
            this.btSave.Click += new System.EventHandler(this.btSave_Click);
            // 
            // btClose
            // 
            this.btClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btClose.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btClose.Location = new System.Drawing.Point(498, 4);
            this.btClose.Name = "btClose";
            this.btClose.Size = new System.Drawing.Size(75, 23);
            this.btClose.TabIndex = 104;
            this.btClose.Tag = "Close";
            this.btClose.Text = "Close";
            this.btClose.UseVisualStyleBackColor = true;
            this.btClose.Click += new System.EventHandler(this.closeSubset_Chosen);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.ctrlPreview, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.pNamedParams, 0, 1);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(-2, 25);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 0F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(246, 603);
            this.tableLayoutPanel1.TabIndex = 31;
            // 
            // ctrlPreview
            // 
            this.ctrlPreview.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.ctrlPreview.Location = new System.Drawing.Point(3, 3);
            this.ctrlPreview.Name = "ctrlPreview";
            this.ctrlPreview.Size = new System.Drawing.Size(240, 597);
            this.ctrlPreview.TabIndex = 32;
            // 
            // pNamedParams
            // 
            this.pNamedParams.Controls.Add(this.pInnerNamedParams);
            this.pNamedParams.Controls.Add(this.lbNamedParams);
            this.pNamedParams.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pNamedParams.Location = new System.Drawing.Point(3, 606);
            this.pNamedParams.Name = "pNamedParams";
            this.pNamedParams.Size = new System.Drawing.Size(240, 1);
            this.pNamedParams.TabIndex = 110;
            this.pNamedParams.Visible = false;
            this.pNamedParams.VisibleChanged += new System.EventHandler(this.pNamedParams_VisibleChanged);
            // 
            // pInnerNamedParams
            // 
            this.pInnerNamedParams.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pInnerNamedParams.AutoScroll = true;
            this.pInnerNamedParams.Location = new System.Drawing.Point(3, 22);
            this.pInnerNamedParams.Name = "pInnerNamedParams";
            this.pInnerNamedParams.Size = new System.Drawing.Size(234, 0);
            this.pInnerNamedParams.TabIndex = 111;
            // 
            // lbNamedParams
            // 
            this.lbNamedParams.AutoSize = true;
            this.lbNamedParams.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbNamedParams.Location = new System.Drawing.Point(0, 0);
            this.lbNamedParams.Name = "lbNamedParams";
            this.lbNamedParams.Size = new System.Drawing.Size(113, 13);
            this.lbNamedParams.TabIndex = 0;
            this.lbNamedParams.Text = "Named Parameters";
            // 
            // lbPreview
            // 
            this.lbPreview.AutoSize = true;
            this.lbPreview.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbPreview.Location = new System.Drawing.Point(1, 9);
            this.lbPreview.Name = "lbPreview";
            this.lbPreview.Size = new System.Drawing.Size(52, 13);
            this.lbPreview.TabIndex = 0;
            this.lbPreview.Tag = "Preview";
            this.lbPreview.Text = "Preview";
            // 
            // tmTimer
            // 
            this.tmTimer.Tick += new System.EventHandler(this.tmTimer_Tick);
            // 
            // tTip
            // 
            this.tTip.AutomaticDelay = 1000;
            this.tTip.AutoPopDelay = 20000;
            this.tTip.InitialDelay = 1000;
            this.tTip.IsBalloon = true;
            this.tTip.ReshowDelay = 200;
            this.tTip.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.tTip.ToolTipTitle = "Description";
            // 
            // SubSetEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btClose;
            this.ClientSize = new System.Drawing.Size(995, 653);
            this.Controls.Add(this.tspMain);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MinimumSize = new System.Drawing.Size(694, 584);
            this.Name = "SubSetEditor";
            this.Text = "SubSetEditor";
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.SubSetEditor_KeyUp);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SubSetEditor_FormClosing);
            this.tspMain.BottomToolStripPanel.ResumeLayout(false);
            this.tspMain.BottomToolStripPanel.PerformLayout();
            this.tspMain.ContentPanel.ResumeLayout(false);
            this.tspMain.ResumeLayout(false);
            this.tspMain.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.scVertical1.Panel1.ResumeLayout(false);
            this.scVertical1.Panel1.PerformLayout();
            this.scVertical1.Panel2.ResumeLayout(false);
            this.scVertical1.ResumeLayout(false);
            this.scVertical2.Panel1.ResumeLayout(false);
            this.scVertical2.Panel2.ResumeLayout(false);
            this.scVertical2.Panel2.PerformLayout();
            this.scVertical2.ResumeLayout(false);
            this.scMain.Panel1.ResumeLayout(false);
            this.scMain.Panel2.ResumeLayout(false);
            this.scMain.Panel2.PerformLayout();
            this.scMain.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.pNamedParams.ResumeLayout(false);
            this.pNamedParams.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStripContainer tspMain;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel statusBar;
        private System.Windows.Forms.Timer tmTimer;
        private System.Windows.Forms.SplitContainer scMain;
        private System.Windows.Forms.Button btSave;
        private System.Windows.Forms.Button btClose;
        private System.Windows.Forms.Button btCopy;
        private System.Windows.Forms.Button btPaste;
        private System.Windows.Forms.ToolTip tTip;
        private System.Windows.Forms.SplitContainer scVertical1;
        private System.Windows.Forms.SplitContainer scVertical2;
        public Apalo.XlAddin.JSubSet.ASubSet aSubSet;
        private System.Windows.Forms.Label lbStoredSubsets;
        private System.Windows.Forms.Label lbPreview;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private Apalo.XlAddin.JSubSet.CtrlPreview ctrlPreview;
        private System.Windows.Forms.Panel pNamedParams;
        private Apalo.XlAddin.JSubSet.Controls.CStoredSubsets cSubsets;
        private System.Windows.Forms.Label lbNamedParams;
        private System.Windows.Forms.Panel pInnerNamedParams;


    }
}
