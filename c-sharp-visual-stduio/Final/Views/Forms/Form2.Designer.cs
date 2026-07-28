namespace Final.Views.Forms
{
    partial class Form2
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
            this.label1 = new System.Windows.Forms.Label();
            this.btCloseFm = new System.Windows.Forms.Button();
            this.btCloseApp = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(352, 100);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "Form 2";
            // 
            // btCloseFm
            // 
            this.btCloseFm.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btCloseFm.ForeColor = System.Drawing.Color.DarkOrange;
            this.btCloseFm.Location = new System.Drawing.Point(518, 381);
            this.btCloseFm.Name = "btCloseFm";
            this.btCloseFm.Size = new System.Drawing.Size(132, 57);
            this.btCloseFm.TabIndex = 1;
            this.btCloseFm.Text = "Close Form";
            this.btCloseFm.UseVisualStyleBackColor = true;
            this.btCloseFm.Click += new System.EventHandler(this.btCloseFm_Click);
            // 
            // btCloseApp
            // 
            this.btCloseApp.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btCloseApp.Location = new System.Drawing.Point(656, 381);
            this.btCloseApp.Name = "btCloseApp";
            this.btCloseApp.Size = new System.Drawing.Size(132, 57);
            this.btCloseApp.TabIndex = 2;
            this.btCloseApp.Text = "Close App";
            this.btCloseApp.UseVisualStyleBackColor = true;
            this.btCloseApp.Click += new System.EventHandler(this.btCloseApp_Click);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.btCloseApp);
            this.Controls.Add(this.btCloseFm);
            this.Controls.Add(this.label1);
            this.Name = "Form2";
            this.Text = "Form2";
            this.Load += new System.EventHandler(this.Form2_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btCloseFm;
        private System.Windows.Forms.Button btCloseApp;
    }
}