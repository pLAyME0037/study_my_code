namespace Homework.Views.Forms
{
    partial class ValidationForm
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
            this.textBoxData = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btCheckDecimal = new System.Windows.Forms.Button();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.btCheckPosDecimal = new System.Windows.Forms.Button();
            this.btCheckInt = new System.Windows.Forms.Button();
            this.btCheckPosInt = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxResult = new System.Windows.Forms.TextBox();
            this.btBack = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.btCheckDicimalDef = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBoxData
            // 
            this.textBoxData.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxData.Location = new System.Drawing.Point(215, 60);
            this.textBoxData.Name = "textBoxData";
            this.textBoxData.Size = new System.Drawing.Size(440, 38);
            this.textBoxData.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(135, 63);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(74, 32);
            this.label1.TabIndex = 1;
            this.label1.Text = "Data";
            // 
            // btCheckDecimal
            // 
            this.btCheckDecimal.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btCheckDecimal.Location = new System.Drawing.Point(141, 162);
            this.btCheckDecimal.Name = "btCheckDecimal";
            this.btCheckDecimal.Size = new System.Drawing.Size(204, 49);
            this.btCheckDecimal.TabIndex = 2;
            this.btCheckDecimal.Text = "Check";
            this.btCheckDecimal.UseVisualStyleBackColor = true;
            this.btCheckDecimal.Click += new System.EventHandler(this.btCheckDecimal_Click);
            // 
            // btCheckPosDecimal
            // 
            this.btCheckPosDecimal.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btCheckPosDecimal.Location = new System.Drawing.Point(141, 217);
            this.btCheckPosDecimal.Name = "btCheckPosDecimal";
            this.btCheckPosDecimal.Size = new System.Drawing.Size(204, 49);
            this.btCheckPosDecimal.TabIndex = 3;
            this.btCheckPosDecimal.Text = "Check";
            this.btCheckPosDecimal.UseVisualStyleBackColor = true;
            this.btCheckPosDecimal.Click += new System.EventHandler(this.btCheckPosDecimal_Click);
            // 
            // btCheckInt
            // 
            this.btCheckInt.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btCheckInt.Location = new System.Drawing.Point(141, 272);
            this.btCheckInt.Name = "btCheckInt";
            this.btCheckInt.Size = new System.Drawing.Size(204, 49);
            this.btCheckInt.TabIndex = 4;
            this.btCheckInt.Text = "Check";
            this.btCheckInt.UseVisualStyleBackColor = true;
            this.btCheckInt.Click += new System.EventHandler(this.btCheckInt_Click);
            // 
            // btCheckPosInt
            // 
            this.btCheckPosInt.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btCheckPosInt.Location = new System.Drawing.Point(141, 327);
            this.btCheckPosInt.Name = "btCheckPosInt";
            this.btCheckPosInt.Size = new System.Drawing.Size(204, 49);
            this.btCheckPosInt.TabIndex = 5;
            this.btCheckPosInt.Text = "Check";
            this.btCheckPosInt.UseVisualStyleBackColor = true;
            this.btCheckPosInt.Click += new System.EventHandler(this.btCheckPosInt_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(351, 168);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(229, 36);
            this.label2.TabIndex = 6;
            this.label2.Text = "Decimal (2 digit)";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(351, 223);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(402, 36);
            this.label3.TabIndex = 7;
            this.label3.Text = "Positive Decimal (1 or 2 digit)";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(351, 278);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(107, 36);
            this.label4.TabIndex = 8;
            this.label4.Text = "Intager";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(351, 333);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(220, 36);
            this.label5.TabIndex = 9;
            this.label5.Text = "Positive Intager";
            // 
            // textBoxResult
            // 
            this.textBoxResult.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxResult.Location = new System.Drawing.Point(141, 382);
            this.textBoxResult.Name = "textBoxResult";
            this.textBoxResult.Size = new System.Drawing.Size(514, 38);
            this.textBoxResult.TabIndex = 10;
            // 
            // btBack
            // 
            this.btBack.Font = new System.Drawing.Font("Consolas", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btBack.ForeColor = System.Drawing.Color.OrangeRed;
            this.btBack.Location = new System.Drawing.Point(12, 12);
            this.btBack.Name = "btBack";
            this.btBack.Size = new System.Drawing.Size(74, 32);
            this.btBack.TabIndex = 11;
            this.btBack.Text = "Back";
            this.btBack.UseVisualStyleBackColor = true;
            this.btBack.Click += new System.EventHandler(this.btBack_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(351, 113);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(120, 36);
            this.label6.TabIndex = 13;
            this.label6.Text = "Decimal";
            // 
            // btCheckDicimalDef
            // 
            this.btCheckDicimalDef.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btCheckDicimalDef.Location = new System.Drawing.Point(141, 107);
            this.btCheckDicimalDef.Name = "btCheckDicimalDef";
            this.btCheckDicimalDef.Size = new System.Drawing.Size(204, 49);
            this.btCheckDicimalDef.TabIndex = 12;
            this.btCheckDicimalDef.Text = "Check";
            this.btCheckDicimalDef.UseVisualStyleBackColor = true;
            this.btCheckDicimalDef.Click += new System.EventHandler(this.btCheckDicimalDef_Click);
            // 
            // ValidationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.btCheckDicimalDef);
            this.Controls.Add(this.btBack);
            this.Controls.Add(this.textBoxResult);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btCheckPosInt);
            this.Controls.Add(this.btCheckInt);
            this.Controls.Add(this.btCheckPosDecimal);
            this.Controls.Add(this.btCheckDecimal);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxData);
            this.Name = "ValidationForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Validation";
            this.Load += new System.EventHandler(this.ValidationForm_Load_1);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxData;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btCheckDecimal;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.Button btCheckPosDecimal;
        private System.Windows.Forms.Button btCheckInt;
        private System.Windows.Forms.Button btCheckPosInt;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxResult;
        private System.Windows.Forms.Button btBack;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btCheckDicimalDef;
    }
}