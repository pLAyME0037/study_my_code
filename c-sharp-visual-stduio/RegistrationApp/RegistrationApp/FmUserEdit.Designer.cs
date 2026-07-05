
namespace RegistrationApp
{
    partial class FmUserEdit
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
            this.TxtFullname = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.CbUserType = new System.Windows.Forms.ComboBox();
            this.CbSex = new System.Windows.Forms.ComboBox();
            this.TxtUsername = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.BtChange = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // TxtFullname
            // 
            this.TxtFullname.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtFullname.Location = new System.Drawing.Point(337, 157);
            this.TxtFullname.Name = "TxtFullname";
            this.TxtFullname.Size = new System.Drawing.Size(359, 34);
            this.TxtFullname.TabIndex = 14;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(77, 213);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(54, 29);
            this.label4.TabIndex = 19;
            this.label4.Text = "Sex";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(77, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(125, 29);
            this.label1.TabIndex = 18;
            this.label1.Text = "User Type";
            // 
            // CbUserType
            // 
            this.CbUserType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CbUserType.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CbUserType.FormattingEnabled = true;
            this.CbUserType.Location = new System.Drawing.Point(337, 50);
            this.CbUserType.Name = "CbUserType";
            this.CbUserType.Size = new System.Drawing.Size(359, 37);
            this.CbUserType.TabIndex = 12;
            // 
            // CbSex
            // 
            this.CbSex.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CbSex.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CbSex.FormattingEnabled = true;
            this.CbSex.Location = new System.Drawing.Point(337, 207);
            this.CbSex.Name = "CbSex";
            this.CbSex.Size = new System.Drawing.Size(359, 37);
            this.CbSex.TabIndex = 15;
            // 
            // TxtUsername
            // 
            this.TxtUsername.Enabled = false;
            this.TxtUsername.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtUsername.Location = new System.Drawing.Point(337, 104);
            this.TxtUsername.Name = "TxtUsername";
            this.TxtUsername.Size = new System.Drawing.Size(359, 34);
            this.TxtUsername.TabIndex = 13;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(77, 160);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(113, 29);
            this.label3.TabIndex = 17;
            this.label3.Text = "Fullname";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(78, 107);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(124, 29);
            this.label2.TabIndex = 16;
            this.label2.Text = "Username";
            // 
            // BtChange
            // 
            this.BtChange.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtChange.Location = new System.Drawing.Point(278, 286);
            this.BtChange.Name = "BtChange";
            this.BtChange.Size = new System.Drawing.Size(216, 69);
            this.BtChange.TabIndex = 21;
            this.BtChange.Text = "Change";
            this.BtChange.UseVisualStyleBackColor = true;
            this.BtChange.Click += new System.EventHandler(this.BtChange_Click);
            // 
            // FmUserEdit
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(782, 398);
            this.Controls.Add(this.BtChange);
            this.Controls.Add(this.TxtFullname);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.CbUserType);
            this.Controls.Add(this.CbSex);
            this.Controls.Add(this.TxtUsername);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FmUserEdit";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Edit User Form";
            this.Load += new System.EventHandler(this.FmUserEdit_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TxtFullname;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox CbUserType;
        private System.Windows.Forms.ComboBox CbSex;
        private System.Windows.Forms.TextBox TxtUsername;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button BtChange;
    }
}