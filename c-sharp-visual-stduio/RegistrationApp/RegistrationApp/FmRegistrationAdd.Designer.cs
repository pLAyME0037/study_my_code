
namespace RegistrationApp
{
    partial class FmRegistrationAdd
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
            this.BtRegister = new System.Windows.Forms.Button();
            this.BtClear = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.TxtStudentCode = new System.Windows.Forms.TextBox();
            this.TxtName = new System.Windows.Forms.TextBox();
            this.CbSex = new System.Windows.Forms.ComboBox();
            this.DtpDoB = new System.Windows.Forms.DateTimePicker();
            this.SuspendLayout();
            // 
            // BtRegister
            // 
            this.BtRegister.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtRegister.Location = new System.Drawing.Point(407, 271);
            this.BtRegister.Name = "BtRegister";
            this.BtRegister.Size = new System.Drawing.Size(236, 68);
            this.BtRegister.TabIndex = 6;
            this.BtRegister.Text = "Register";
            this.BtRegister.UseVisualStyleBackColor = true;
            this.BtRegister.Click += new System.EventHandler(this.BtRegister_Click);
            // 
            // BtClear
            // 
            this.BtClear.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtClear.Location = new System.Drawing.Point(165, 271);
            this.BtClear.Name = "BtClear";
            this.BtClear.Size = new System.Drawing.Size(236, 68);
            this.BtClear.TabIndex = 5;
            this.BtClear.Text = "Clear";
            this.BtClear.UseVisualStyleBackColor = true;
            this.BtClear.Click += new System.EventHandler(this.BtClear_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(148, 58);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(154, 29);
            this.label1.TabIndex = 2;
            this.label1.Text = "StudentCode";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(148, 111);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 29);
            this.label2.TabIndex = 3;
            this.label2.Text = "Name";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(148, 164);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 29);
            this.label3.TabIndex = 4;
            this.label3.Text = "Sex";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(148, 215);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 29);
            this.label4.TabIndex = 5;
            this.label4.Text = "DoB";
            // 
            // TxtStudentCode
            // 
            this.TxtStudentCode.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtStudentCode.Location = new System.Drawing.Point(304, 57);
            this.TxtStudentCode.Name = "TxtStudentCode";
            this.TxtStudentCode.Size = new System.Drawing.Size(359, 34);
            this.TxtStudentCode.TabIndex = 1;
            // 
            // TxtName
            // 
            this.TxtName.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtName.Location = new System.Drawing.Point(304, 106);
            this.TxtName.Name = "TxtName";
            this.TxtName.Size = new System.Drawing.Size(359, 34);
            this.TxtName.TabIndex = 2;
            // 
            // CbSex
            // 
            this.CbSex.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CbSex.FormattingEnabled = true;
            this.CbSex.Location = new System.Drawing.Point(304, 156);
            this.CbSex.Name = "CbSex";
            this.CbSex.Size = new System.Drawing.Size(359, 37);
            this.CbSex.TabIndex = 3;
            // 
            // DtpDoB
            // 
            this.DtpDoB.CustomFormat = "dd/MMM/yyyy";
            this.DtpDoB.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DtpDoB.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.DtpDoB.Location = new System.Drawing.Point(304, 210);
            this.DtpDoB.Name = "DtpDoB";
            this.DtpDoB.Size = new System.Drawing.Size(359, 34);
            this.DtpDoB.TabIndex = 4;
            this.DtpDoB.Value = new System.DateTime(2025, 10, 9, 0, 0, 0, 0);
            // 
            // FmRegistrationAdd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(831, 371);
            this.Controls.Add(this.DtpDoB);
            this.Controls.Add(this.CbSex);
            this.Controls.Add(this.TxtName);
            this.Controls.Add(this.TxtStudentCode);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.BtClear);
            this.Controls.Add(this.BtRegister);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FmRegistrationAdd";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Register Form";
            this.Load += new System.EventHandler(this.FmRegistrationAdd_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button BtRegister;
        private System.Windows.Forms.Button BtClear;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox TxtStudentCode;
        private System.Windows.Forms.TextBox TxtName;
        private System.Windows.Forms.ComboBox CbSex;
        private System.Windows.Forms.DateTimePicker DtpDoB;
    }
}