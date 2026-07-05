using Homework.Models;
using System;
using System.Windows.Forms;

namespace Homework.Views.Forms
{
    public partial class UsingConvertToForm : Form
    {
        private readonly UsingConvertTo converter = new UsingConvertTo();

        public UsingConvertToForm()
        {
            InitializeComponent();
        }

        private void UsingConvertToForm_Load(object sender, EventArgs e)
        {
            this.Text = "Using Convert To";
            if (comboBoxTargetType.Items.Count > 0)
                comboBoxTargetType.SelectedIndex = 0;
        }

        private void buttonConvert_Click(object sender, EventArgs e)
        {
            string input = textBoxInput.Text;
            string target = comboBoxTargetType.SelectedItem as string;
            textBoxResult.Clear();

            try
            {
                switch (target)
                {
                    case "int":
                    {
                        int result = converter.ConvertToInt(input);
                        textBoxResult.Text = $"{result} (System.{result.GetType().Name})";
                        break;
                    }
                    case "float":
                    {
                        float result = converter.ConvertToFloat(input);
                        textBoxResult.Text = $"{result} (System.{result.GetType().Name})";
                        break;
                    }
                    case "decimal":
                    {
                        decimal result = converter.ConvertToDecimal(input);
                        textBoxResult.Text = $"{result} (System.{result.GetType().Name})";
                        break;
                    }
                    case "string":
                    {
                        string result = converter.ConvertToString(input);
                        textBoxResult.Text = $"\"{result}\" (System.{result.GetType().Name})";
                        break;
                    }
                    case "char":
                    {
                        char result = converter.ConvertToChar(input);
                        textBoxResult.Text = $"'{result}' (System.{result.GetType().Name})";
                        break;
                    }
                    case "boolean":
                    {
                        bool result = converter.ConvertToBool(input);
                        textBoxResult.Text = $"{result} (System.{result.GetType().Name})";
                        break;
                    }
                    default:
                        textBoxResult.Text = "Select a target type.";
                        break;
                }
            }
            catch (FormatException fex)
            {
                MessageBox.Show("Format error: " + fex.Message, "Conversion Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            catch (OverflowException oex)
            {
                MessageBox.Show("Overflow error: " + oex.Message, "Conversion Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            catch (ArgumentNullException anex)
            {
                MessageBox.Show("Null input: " + anex.Message, "Conversion Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected error: " + ex.Message, "Conversion Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Sample buttons to populate input and type
        private void btnSampleInt_Click(object sender, EventArgs e)
        {
            textBoxInput.Text = "42";
            comboBoxTargetType.SelectedItem = "int";
        }

        private void btnSampleFloat_Click(object sender, EventArgs e)
        {
            textBoxInput.Text = "3.14";
            comboBoxTargetType.SelectedItem = "float";
        }

        private void btnSampleDecimal_Click(object sender, EventArgs e)
        {
            textBoxInput.Text = "12345.678";
            comboBoxTargetType.SelectedItem = "decimal";
        }

        private void btnSampleString_Click(object sender, EventArgs e)
        {
            textBoxInput.Text = "Hello World";
            comboBoxTargetType.SelectedItem = "string";
        }

        private void btnSampleChar_Click(object sender, EventArgs e)
        {
            textBoxInput.Text = "X";
            comboBoxTargetType.SelectedItem = "char";
        }

        private void btnSampleBool_Click(object sender, EventArgs e)
        {
            textBoxInput.Text = "True";
            comboBoxTargetType.SelectedItem = "boolean";
        }
    }
}
