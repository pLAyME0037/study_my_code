
namespace RegistrationApp
{
    partial class FmUserChangePassword
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
            this.BtChange = new System.Windows.Forms.Button();
            this.TxtNewConfirmedPassword = new System.Windows.Forms.TextBox();
            this.TxtNewPassword = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.TxtPassword = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // BtChange
            // 
            this.BtChange.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BtChange.Location = new System.Drawing.Point(291, 282);
            this.BtChange.Name = "BtChange";
            this.BtChange.Size = new System.Drawing.Size(258, 69);
            this.BtChange.TabIndex = 3;
            this.BtChange.Text = "Change Password";
            this.BtChange.UseVisualStyleBackColor = true;
            this.BtChange.Click += new System.EventHandler(this.BtChange_Click);
            // 
            // TxtNewConfirmedPassword
            // 
            this.TxtNewConfirmedPassword.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtNewConfirmedPassword.Location = new System.Drawing.Point(371, 117);
            this.TxtNewConfirmedPassword.Name = "TxtNewConfirmedPassword";
            this.TxtNewConfirmedPassword.PasswordChar = '*';
            this.TxtNewConfirmedPassword.Size = new System.Drawing.Size(359, 34);
            this.TxtNewConfirmedPassword.TabIndex = 1;
            // 
            // TxtNewPassword
            // 
            this.TxtNewPassword.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtNewPassword.Location = new System.Drawing.Point(371, 64);
            this.TxtNewPassword.Name = "TxtNewPassword";
            this.TxtNewPassword.PasswordChar = '*';
            this.TxtNewPassword.Size = new System.Drawing.Size(359, 34);
            this.TxtNewPassword.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(67, 117);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(294, 29);
            this.label3.TabIndex = 25;
            this.label3.Text = "New Confirmed Password";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(67, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(176, 29);
            this.label2.TabIndex = 24;
            this.label2.Text = "New Password";
            // 
            // TxtPassword
            // 
            this.TxtPassword.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtPassword.Location = new System.Drawing.Point(371, 196);
            this.TxtPassword.Name = "TxtPassword";
            this.TxtPassword.PasswordChar = '*';
            this.TxtPassword.Size = new System.Drawing.Size(359, 34);
            this.TxtPassword.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(67, 196);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(120, 29);
            this.label1.TabIndex = 28;
            this.label1.Text = "Password";
            // 
            // FmUserChangePassword
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 388);
            this.Controls.Add(this.TxtPassword);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.BtChange);
            this.Controls.Add(this.TxtNewConfirmedPassword);
            this.Controls.Add(this.TxtNewPassword);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FmUserChangePassword";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Change Password Form";
            this.Load += new System.EventHandler(this.FmUserChangePassword_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button BtChange;
        private System.Windows.Forms.TextBox TxtNewConfirmedPassword;
        private System.Windows.Forms.TextBox TxtNewPassword;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox TxtPassword;
        private System.Windows.Forms.Label label1;
    }
}