import 'package:flutter/material.dart';
import '../services/auth_service.dart';

class CalculatorScreen extends StatefulWidget {
  const CalculatorScreen({super.key});

  @override
  State<CalculatorScreen> createState() => _CalculatorScreenState();
}

class _CalculatorScreenState extends State<CalculatorScreen> {
  final _oldCtrl = TextEditingController();
  final _newCtrl = TextEditingController();
  final _kwCtrl = TextEditingController();
  final _auth = AuthService();

  String? _result;

  @override
  void initState() {
    super.initState();
    _newCtrl.addListener(_updateKw);
    _oldCtrl.addListener(_updateKw);
  }

  void _updateKw() {
    final old = double.tryParse(_oldCtrl.text) ?? 0;
    final newVal = double.tryParse(_newCtrl.text) ?? 0;
    _kwCtrl.text = (newVal - old).toStringAsFixed(0);
  }

  @override
  void dispose() {
    _oldCtrl.removeListener(_updateKw);
    _newCtrl.removeListener(_updateKw);
    _oldCtrl.dispose();
    _newCtrl.dispose();
    _kwCtrl.dispose();
    super.dispose();
  }

  void _calculate() {
    final kw = double.tryParse(_kwCtrl.text) ?? 0;
    if (kw <= 0) {
      setState(() => _result = null);
      return;
    }

    int rate;
    if (kw < 50) {
      rate = 650;
    } else if (kw <= 80) {
      rate = 750;
    } else if (kw <= 150) {
      rate = 850;
    } else {
      rate = 950;
    }

    final payment = kw * rate;
    final user = _auth.currentUser!;
    setState(() {
      _result =
          'name: ${user.username}\r\npower total : ${kw.toStringAsFixed(0)}\r\npayment : ${payment.toStringAsFixed(0)} \Riel';
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Electricity Bill Calculator')),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            TextField(
              controller: _oldCtrl,
              decoration: const InputDecoration(labelText: 'Old Number'),
              keyboardType: TextInputType.number,
            ),
            TextField(
              controller: _newCtrl,
              decoration: const InputDecoration(labelText: 'New Number'),
              keyboardType: TextInputType.number,
            ),
            TextField(
              controller: _kwCtrl,
              decoration: const InputDecoration(labelText: 'Amount (KW)'),
              keyboardType: TextInputType.number,
              enabled: false,
            ),
            const SizedBox(height: 16),
            ElevatedButton(
              onPressed: _calculate,
              child: const Text('Calculate'),
            ),
            const SizedBox(height: 16),
            if (_result != null)
              Card(
                child: Padding(
                  padding: const EdgeInsets.all(16),
                  child: Text(_result!),
                ),
              ),
          ],
        ),
      ),
    );
  }
}
