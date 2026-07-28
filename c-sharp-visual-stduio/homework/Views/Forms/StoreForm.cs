using Homework.Helpers;
using System;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Homework.Views.Forms
{
    public partial class StoreForm : Form
    {
        public int priceIs = 0;
        public int binaryInput = 0;

        public StoreForm()
        {
            InitializeComponent();

            this.buttonBuyApple.Click += (s, e) => AddPriceFromLabel(this.label1000);
            this.buttonBuyCocacola.Click += (s, e) => AddPriceFromLabel(this.label2000);
            this.buttonBuyShirt.Click += (s, e) => AddPriceFromLabel(this.label10000);
            this.buttonBuySticker.Click += (s, e) => AddPriceFromLabel(this.label500);
            this.buttonBuyBattery.Click += (s, e) => AddPriceFromLabel(this.label5000);

            // Initialize display
            UpdatePriceText();
        }
        private void AddPriceFromLabel(Label label)
        {
            if (label == null) return;
            if (int.TryParse(label.Text, out int value))
            {
                AddPrice(value);
            }
            else
            {
                MessageBox.Show("Invalid Price");
            }
        }

        private void AddPrice(int amount)
        {
            priceIs += amount;
            UpdatePriceText();
        }


        // Updates the TextBox that shows the current total price.
        private void UpdatePriceText()
        {
            if (this.textBoxPriceIs != null)
            {
                this.textBoxPriceIs.Text = priceIs.ToString();
                this.textBoxPriceUSD.Text = priceUsd(this.priceIs).ToString();
            }
        }

        private void Store_Load(object sender, EventArgs e)
        {
            this.Text = "Store";
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            this.priceIs = 0;
            UpdatePriceText();
        }

        private decimal priceUsd(decimal priceUsd)
        {
            priceUsd /= 4000;
            return priceUsd;
        }

        // Helper: Validate binary input
        static bool IsValidBinary(string input)
        {
            return input.All(c => c == '0' || c == '1');
        }

        private void buttonConvert_Click(object sender, EventArgs e)
        {
            string binaryInput = this.textBoxInputBinary.Text.Trim();

            // Clear previous output
            this.textBoxConverted.Text = string.Empty;

            if (string.IsNullOrEmpty(binaryInput) || !IsValidBinary(binaryInput))
            {
                MessageBox.Show("Invalid input! Use only 0s and 1s.");
                return;
            }

            StringBuilder output = new StringBuilder();

            // Step 1: Convert binary string to decimal using ToInt32()
            int decimalValue = Convert.ToInt32(binaryInput, 2);  // Base 2 for binary
            output.AppendLine($"Binary '{binaryInput}' → Decimal: {decimalValue}");

            // Step 2: Evaluate truth table using ToBoolean() on each bit
            output.AppendLine("\nTruth Table (Bit → Boolean):");
            bool overallTruth = false;
            for (int i = 0; i < binaryInput.Length; i++)
            {
                // FIX 2: Check for potential ArgumentOutOfRangeException if binaryInput is too long
                if (i >= binaryInput.Length) break;

                int bit = int.Parse(binaryInput[i].ToString());
                bool isTrue = Convert.ToBoolean(bit);  // Converts 1→true, 0→false
                overallTruth = overallTruth || isTrue;  // OR all bits for "overall truth"
                output.AppendLine($"  Bit {i}: {bit} → {isTrue}");
            }
            output.AppendLine($"Overall Truth (OR of bits): {overallTruth}");

            // Step 3: Convert decimal to char using ToChar()
            char asciiChar;
            try
            {
                asciiChar = Convert.ToChar(decimalValue);  // e.g., 65 → 'A'
                output.AppendLine($"\nDecimal {decimalValue} → ASCII Char: '{asciiChar}'");
            }
            catch (OverflowException)
            {
                // Display the output collected so far before showing the error.
                this.textBoxConverted.Text = output.ToString();
                MessageBox.Show("Decimal too big for ASCII! (Max ~127 for printable chars)");
                return;
            }

            // Display all calculated output in the designated TextBox
            this.textBoxConverted.Text = output.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            NavigationManager.NavigateBack(this);
        }
    }
}

