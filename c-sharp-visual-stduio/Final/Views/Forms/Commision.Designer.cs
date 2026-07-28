namespace Final.Views.Forms
{
    partial class Commision
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
            this.txbSale = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txbCommision = new System.Windows.Forms.TextBox();
            this.btChkCom = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("CaskaydiaMono NF", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(198, 84);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 24);
            this.label1.TabIndex = 0;
            this.label1.Text = "Sale";
            // 
            // txbSale
            // 
            this.txbSale.Font = new System.Drawing.Font("CaskaydiaMono NF", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txbSale.Location = new System.Drawing.Point(311, 81);
            this.txbSale.Name = "txbSale";
            this.txbSale.Size = new System.Drawing.Size(230, 31);
            this.txbSale.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("CaskaydiaMono NF", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(160, 127);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(130, 24);
            this.label2.TabIndex = 2;
            this.label2.Text = "Commission";
            // 
            // txbCommision
            // 
            this.txbCommision.Font = new System.Drawing.Font("CaskaydiaMono NF", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txbCommision.Location = new System.Drawing.Point(311, 120);
            this.txbCommision.Name = "txbCommision";
            this.txbCommision.Size = new System.Drawing.Size(230, 31);
            this.txbCommision.TabIndex = 3;
            // 
            // btChkCom
            // 
            this.btChkCom.Font = new System.Drawing.Font("CaskaydiaMono NF", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btChkCom.Location = new System.Drawing.Point(267, 215);
            this.btChkCom.Name = "btChkCom";
            this.btChkCom.Size = new System.Drawing.Size(168, 64);
            this.btChkCom.TabIndex = 4;
            this.btChkCom.Text = "Check Commision";
            this.btChkCom.UseVisualStyleBackColor = true;
            this.btChkCom.Click += new System.EventHandler(this.btChkCom_Click);
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("CaskaydiaMono NF", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.Location = new System.Drawing.Point(603, 354);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(141, 54);
            this.button1.TabIndex = 5;
            this.button1.Text = "Back";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Commision
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btChkCom);
            this.Controls.Add(this.txbCommision);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txbSale);
            this.Controls.Add(this.label1);
            this.Name = "Commision";
            this.Text = "Commision";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txbSale;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txbCommision;
        private System.Windows.Forms.Button btChkCom;
        private System.Windows.Forms.Button button1;
    }
}