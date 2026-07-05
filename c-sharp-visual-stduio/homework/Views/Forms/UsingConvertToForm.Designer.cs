namespace Homework.Views.Forms
{
    partial class UsingConvertToForm
    {
        private System.ComponentModel.IContainer components = null;

        private System.Windows.Forms.Label labelInput;
        private System.Windows.Forms.TextBox textBoxInput;
        private System.Windows.Forms.Label labelType;
        private System.Windows.Forms.ComboBox comboBoxTargetType;
        private System.Windows.Forms.Button buttonConvert;
        private System.Windows.Forms.Label labelResult;
        private System.Windows.Forms.TextBox textBoxResult;
        private System.Windows.Forms.Button btnSampleInt;
        private System.Windows.Forms.Button btnSampleFloat;
        private System.Windows.Forms.Button btnSampleDecimal;
        private System.Windows.Forms.Button btnSampleString;
        private System.Windows.Forms.Button btnSampleChar;
        private System.Windows.Forms.Button btnSampleBool;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.labelInput = new System.Windows.Forms.Label();
            this.textBoxInput = new System.Windows.Forms.TextBox();
            this.labelType = new System.Windows.Forms.Label();
            this.comboBoxTargetType = new System.Windows.Forms.ComboBox();
            this.buttonConvert = new System.Windows.Forms.Button();
            this.labelResult = new System.Windows.Forms.Label();
            this.textBoxResult = new System.Windows.Forms.TextBox();
            this.btnSampleInt = new System.Windows.Forms.Button();
            this.btnSampleFloat = new System.Windows.Forms.Button();
            this.btnSampleDecimal = new System.Windows.Forms.Button();
            this.btnSampleString = new System.Windows.Forms.Button();
            this.btnSampleChar = new System.Windows.Forms.Button();
            this.btnSampleBool = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // labelInput
            // 
            this.labelInput.AutoSize = true;
            this.labelInput.Location = new System.Drawing.Point(16, 18);
            this.labelInput.Name = "labelInput";
            this.labelInput.Size = new System.Drawing.Size(39, 17);
            this.labelInput.TabIndex = 0;
            this.labelInput.Text = "Input";
            // 
            // textBoxInput
            // 
            this.textBoxInput.Location = new System.Drawing.Point(19, 38);
            this.textBoxInput.Name = "textBoxInput";
            this.textBoxInput.Size = new System.Drawing.Size(420, 22);
            this.textBoxInput.TabIndex = 1;
            // 
            // labelType
            // 
            this.labelType.AutoSize = true;
            this.labelType.Location = new System.Drawing.Point(16, 73);
            this.labelType.Name = "labelType";
            this.labelType.Size = new System.Drawing.Size(39, 17);
            this.labelType.TabIndex = 2;
            this.labelType.Text = "Type";
            // 
            // comboBoxTargetType
            // 
            this.comboBoxTargetType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTargetType.FormattingEnabled = true;
            this.comboBoxTargetType.Items.AddRange(new object[] {
                "int",
                "float",
                "decimal",
                "string",
                "char",
                "boolean"});
            this.comboBoxTargetType.Location = new System.Drawing.Point(19, 93);
            this.comboBoxTargetType.Name = "comboBoxTargetType";
            this.comboBoxTargetType.Size = new System.Drawing.Size(200, 24);
            this.comboBoxTargetType.TabIndex = 3;
            // 
            // buttonConvert
            // 
            this.buttonConvert.Location = new System.Drawing.Point(245, 91);
            this.buttonConvert.Name = "buttonConvert";
            this.buttonConvert.Size = new System.Drawing.Size(94, 27);
            this.buttonConvert.TabIndex = 4;
            this.buttonConvert.Text = "Convert";
            this.buttonConvert.UseVisualStyleBackColor = true;
            this.buttonConvert.Click += new System.EventHandler(this.buttonConvert_Click);
            // 
            // labelResult
            // 
            this.labelResult.AutoSize = true;
            this.labelResult.Location = new System.Drawing.Point(16, 130);
            this.labelResult.Name = "labelResult";
            this.labelResult.Size = new System.Drawing.Size(48, 17);
            this.labelResult.TabIndex = 5;
            this.labelResult.Text = "Result";
            // 
            // textBoxResult
            // 
            this.textBoxResult.Location = new System.Drawing.Point(19, 150);
            this.textBoxResult.Multiline = true;
            this.textBoxResult.Name = "textBoxResult";
            this.textBoxResult.ReadOnly = true;
            this.textBoxResult.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxResult.Size = new System.Drawing.Size(420, 150);
            this.textBoxResult.TabIndex = 6;
            // 
            // btnSampleInt
            // 
            this.btnSampleInt.Location = new System.Drawing.Point(460, 36);
            this.btnSampleInt.Name = "btnSampleInt";
            this.btnSampleInt.Size = new System.Drawing.Size(120, 27);
            this.btnSampleInt.TabIndex = 7;
            this.btnSampleInt.Text = "Sample int";
            this.btnSampleInt.UseVisualStyleBackColor = true;
            this.btnSampleInt.Click += new System.EventHandler(this.btnSampleInt_Click);
            // 
            // btnSampleFloat
            // 
            this.btnSampleFloat.Location = new System.Drawing.Point(460, 69);
            this.btnSampleFloat.Name = "btnSampleFloat";
            this.btnSampleFloat.Size = new System.Drawing.Size(120, 27);
            this.btnSampleFloat.TabIndex = 8;
            this.btnSampleFloat.Text = "Sample float";
            this.btnSampleFloat.UseVisualStyleBackColor = true;
            this.btnSampleFloat.Click += new System.EventHandler(this.btnSampleFloat_Click);
            // 
            // btnSampleDecimal
            // 
            this.btnSampleDecimal.Location = new System.Drawing.Point(460, 102);
            this.btnSampleDecimal.Name = "btnSampleDecimal";
            this.btnSampleDecimal.Size = new System.Drawing.Size(120, 27);
            this.btnSampleDecimal.TabIndex = 9;
            this.btnSampleDecimal.Text = "Sample decimal";
            this.btnSampleDecimal.UseVisualStyleBackColor = true;
            this.btnSampleDecimal.Click += new System.EventHandler(this.btnSampleDecimal_Click);
            // 
            // btnSampleString
            // 
            this.btnSampleString.Location = new System.Drawing.Point(460, 135);
            this.btnSampleString.Name = "btnSampleString";
            this.btnSampleString.Size = new System.Drawing.Size(120, 27);
            this.btnSampleString.TabIndex = 10;
            this.btnSampleString.Text = "Sample string";
            this.btnSampleString.UseVisualStyleBackColor = true;
            this.btnSampleString.Click += new System.EventHandler(this.btnSampleString_Click);
            // 
            // btnSampleChar
            // 
            this.btnSampleChar.Location = new System.Drawing.Point(460, 168);
            this.btnSampleChar.Name = "btnSampleChar";
            this.btnSampleChar.Size = new System.Drawing.Size(120, 27);
            this.btnSampleChar.TabIndex = 11;
            this.btnSampleChar.Text = "Sample char";
            this.btnSampleChar.UseVisualStyleBackColor = true;
            this.btnSampleChar.Click += new System.EventHandler(this.btnSampleChar_Click);
            // 
            // btnSampleBool
            // 
            this.btnSampleBool.Location = new System.Drawing.Point(460, 201);
            this.btnSampleBool.Name = "btnSampleBool";
            this.btnSampleBool.Size = new System.Drawing.Size(120, 27);
            this.btnSampleBool.TabIndex = 12;
            this.btnSampleBool.Text = "Sample bool";
            this.btnSampleBool.UseVisualStyleBackColor = true;
            this.btnSampleBool.Click += new System.EventHandler(this.btnSampleBool_Click);
            // 
            // UsingConvertToForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(600, 320);
            this.Controls.Add(this.btnSampleBool);
            this.Controls.Add(this.btnSampleChar);
            this.Controls.Add(this.btnSampleString);
            this.Controls.Add(this.btnSampleDecimal);
            this.Controls.Add(this.btnSampleFloat);
            this.Controls.Add(this.btnSampleInt);
            this.Controls.Add(this.textBoxResult);
            this.Controls.Add(this.labelResult);
            this.Controls.Add(this.buttonConvert);
            this.Controls.Add(this.comboBoxTargetType);
            this.Controls.Add(this.labelType);
            this.Controls.Add(this.textBoxInput);
            this.Controls.Add(this.labelInput);
            this.Name = "UsingConvertToForm";
            this.Text = "UsingConvertToForm";
            this.Load += new System.EventHandler(this.UsingConvertToForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}