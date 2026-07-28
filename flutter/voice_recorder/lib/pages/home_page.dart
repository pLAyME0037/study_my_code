import 'dart:io';

import 'package:flutter/material.dart';
import 'package:just_audio/just_audio.dart';
import 'package:path/path.dart' as p;
import 'package:path_provider/path_provider.dart';
import 'package:record/record.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final AudioRecorder audioRecorder = AudioRecorder();
  final AudioPlayer audioPlayer = AudioPlayer();
  bool isRecording = false;
  bool isPlaying = false;
  String? recordingPath;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      floatingActionButton: _recordingButton(),
      body: _buildUI(),
    );
  }

  Widget _buildUI() {
      return SizedBox(
          width: MediaQuery.sizeOf(context).width,
          child: Column(
              crossAxisAlignment: CrossAxisAlignment.center,
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                  if (recordingPath == null) const Text("No recording found."),
                  if (recordingPath != null) MaterialButton(
                      onPressed: () async {
                          if (audioPlayer.playing) {
                              audioPlayer.stop();
                              setState(() {
                                  isPlaying = false;
                              });
                          } else {
                              await audioPlayer.setFilePath(recordingPath!);
                              audioPlayer.play();
                              setState(() {
                                  isPlaying = true;
                              });
                          }
                      },
                      color: Theme.of(context).colorScheme.primary,
                      child: Text(
                          isPlaying ? "Stop playing recording" : "Start playing recording",
                          style: const TextStyle(
                              color: Colors.white,
                          ),
                      ),
                  ),
              ],
          ),
      );
  }

  Widget _recordingButton() {
      return FloatingActionButton(
          onPressed: () async {
              if (isRecording) {
                  String? filePath = await audioRecorder.stop();
                  if (filePath == null) return;

                  setState(() {
                      isRecording = false;
                      recordingPath = filePath;
                  });

                  return;
              }
               
              if (!await audioRecorder.hasPermission()) return;
              final Directory appDocDir = await getApplicationDocumentsDirectory();
              final String filePath = p.join(appDocDir.path, "recording.wave");
              await audioRecorder.start(
                  const RecordConfig(),
                  path: filePath);
              setState(() {
                  isRecording = true;
                  recordingPath = null;
              });
          },
          child: Icon(isRecording ? Icons.stop: Icons.mic),
      );
  }
}
