
namespace RegistrationApp
{
    partial class FmRegistrationMng
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FmRegistrationMng));
            this.BtDelete = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.BtReport = new System.Windows.Forms.Button();
            this.CmsOnDgv = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.reportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.BtRegistrationList = new System.Windows.Forms.Button();
            this.BtAdd = new System.Windows.Forms.Button();
            this.BtExcel = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.CmsOnDgv.SuspendLayout();
            this.SuspendLayout();
            // 
            // BtDelete
            // 
            this.BtDelete.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtDelete.Location = new System.Drawing.Point(916, 103);
            this.BtDelete.Name = "BtDelete";
            this.BtDelete.Size = new System.Drawing.Size(104, 72);
            this.BtDelete.TabIndex = 2;
            this.BtDelete.Text = "Delete";
            this.BtDelete.UseVisualStyleBackColor = true;
            this.BtDelete.Click += new System.EventHandler(this.BtDelete_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(364, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(475, 46);
            this.label1.TabIndex = 3;
            this.label1.Text = "Registration Management";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridView1.Location = new System.Drawing.Point(12, 197);
            this.dataGridView1.MultiSelect = false;
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowHeadersWidth = 51;
            this.dataGridView1.RowTemplate.Height = 24;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView1.Size = new System.Drawing.Size(1118, 408);
            this.dataGridView1.TabIndex = 4;
            this.dataGridView1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.dataGridView1_MouseClick);
            // 
            // splitter1
            // 
            this.splitter1.Location = new System.Drawing.Point(0, 0);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(3, 613);
            this.splitter1.TabIndex = 5;
            this.splitter1.TabStop = false;
            // 
            // BtReport
            // 
            this.BtReport.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtReport.Location = new System.Drawing.Point(1026, 103);
            this.BtReport.Name = "BtReport";
            this.BtReport.Size = new System.Drawing.Size(104, 72);
            this.BtReport.TabIndex = 6;
            this.BtReport.Text = "Report";
            this.BtReport.UseVisualStyleBackColor = true;
            this.BtReport.Click += new System.EventHandler(this.BtReport_Click);
            // 
            // CmsOnDgv
            // 
            this.CmsOnDgv.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.CmsOnDgv.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.deleteToolStripMenuItem,
            this.reportToolStripMenuItem,
            this.toolStripSeparator1,
            this.refreshToolStripMenuItem});
            this.CmsOnDgv.Name = "CmsOnDgv";
            this.CmsOnDgv.Size = new System.Drawing.Size(128, 82);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(127, 24);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // reportToolStripMenuItem
            // 
            this.reportToolStripMenuItem.Name = "reportToolStripMenuItem";
            this.reportToolStripMenuItem.Size = new System.Drawing.Size(127, 24);
            this.reportToolStripMenuItem.Text = "Report";
            this.reportToolStripMenuItem.Click += new System.EventHandler(this.reportToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(124, 6);
            // 
            // refreshToolStripMenuItem
            // 
            this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
            this.refreshToolStripMenuItem.Size = new System.Drawing.Size(127, 24);
            this.refreshToolStripMenuItem.Text = "Refresh";
            this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
            // 
            // BtRegistrationList
            // 
            this.BtRegistrationList.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtRegistrationList.Image = ((System.Drawing.Image)(resources.GetObject("BtRegistrationList.Image")));
            this.BtRegistrationList.Location = new System.Drawing.Point(98, 104);
            this.BtRegistrationList.Name = "BtRegistrationList";
            this.BtRegistrationList.Size = new System.Drawing.Size(80, 72);
            this.BtRegistrationList.TabIndex = 7;
            this.BtRegistrationList.UseVisualStyleBackColor = true;
            this.BtRegistrationList.Click += new System.EventHandler(this.BtRegistrationList_Click);
            // 
            // BtAdd
            // 
            this.BtAdd.Font = new System.Drawing.Font("Microsoft Sans Serif", 25.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtAdd.Image = ((System.Drawing.Image)(resources.GetObject("BtAdd.Image")));
            this.BtAdd.Location = new System.Drawing.Point(12, 103);
            this.BtAdd.Name = "BtAdd";
            this.BtAdd.Size = new System.Drawing.Size(80, 72);
            this.BtAdd.TabIndex = 0;
            this.BtAdd.UseVisualStyleBackColor = true;
            this.BtAdd.Click += new System.EventHandler(this.BtAdd_Click);
            // 
            // BtExcel
            // 
            this.BtExcel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtExcel.Image = ((System.Drawing.Image)(resources.GetObject("BtExcel.Image")));
            this.BtExcel.Location = new System.Drawing.Point(184, 104);
            this.BtExcel.Name = "BtExcel";
            this.BtExcel.Size = new System.Drawing.Size(80, 72);
            this.BtExcel.TabIndex = 8;
            this.BtExcel.UseVisualStyleBackColor = true;
            this.BtExcel.Click += new System.EventHandler(this.BtExcel_Click);
            // 
            // FmRegistrationMng
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1142, 613);
            this.Controls.Add(this.BtExcel);
            this.Controls.Add(this.BtRegistrationList);
            this.Controls.Add(this.BtReport);
            this.Controls.Add(this.splitter1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.BtDelete);
            this.Controls.Add(this.BtAdd);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FmRegistrationMng";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Registration Management";
            this.Load += new System.EventHandler(this.FmRegistrationMng_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.CmsOnDgv.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button BtAdd;
        private System.Windows.Forms.Button BtDelete;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.Button BtReport;
        private System.Windows.Forms.Button BtRegistrationList;
        private System.Windows.Forms.ContextMenuStrip CmsOnDgv;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem reportToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
        private System.Windows.Forms.Button BtExcel;
    }
}