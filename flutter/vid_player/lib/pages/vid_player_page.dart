import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:video_player/video_player.dart';
import 'package:file_picker/file_picker.dart';
import 'package:shared_preferences/shared_preferences.dart';

class _JsHelpers {
  static String makeUrl(Uint8List bytes) => throw UnimplementedError('web only');
  static void revokeUrl(String url) => throw UnimplementedError('web only');
}

enum VideoSourceType { asset, local }

class VideoListPage extends StatelessWidget {
  const VideoListPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Video Player'),
        backgroundColor: Colors.black87,
        foregroundColor: Colors.white,
      ),
      backgroundColor: Colors.black,
      body: const VideoListBody(),
    );
  }
}

class VideoListBody extends StatefulWidget {
  const VideoListBody({super.key});

  @override
  State<VideoListBody> createState() => _VideoListBodyState();
}

class _VideoListBodyState extends State<VideoListBody> {
  static const String _lastVideoKey = 'last_video_path';
  static const String _lastVideoTypeKey = 'last_video_type';
  static const String _defaultAssetPath = 'videos/Easy trick I use to slice up images in memory (Olive.c Ep.05).mp4';

  final List<Map<String, String>> _videos = [
    {
        'name': 'Olive.c Ep.05',
        'path': 'videos/Easy trick I use to slice up images in memory (Olive.c Ep.05).mp4',
        'type': 'asset'
    },
    {
        'name': 'Angkor Heritage',
        'path': 'videos/Angkor is our Heritage - Khmer with English subtitles.mp4',
        'type': 'asset'
    },
    {
        'name': 'Neon Abstract',
        'path': 'videos/Neon Rounded Purple lines Abstract Gradient Background Animation __ Free Version.mp4',
        'type': 'asset'
    },
  ];

  String? _selectedPath;
  String? _selectedType;

  @override
  void initState() {
    super.initState();
    _loadLastVideo();
  }

  Future<void> _loadLastVideo() async {
    final prefs = await SharedPreferences.getInstance();
    setState(() {
      _selectedPath = prefs.getString(_lastVideoKey);
      _selectedType = prefs.getString(_lastVideoTypeKey);
    });
  }

  Future<void> _saveLastVideo(String path, String type) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString(_lastVideoKey, path);
    await prefs.setString(_lastVideoTypeKey, type);
  }

  Future<void> _pickLocalFile() async {
    if (kIsWeb) {
      final result = await FilePicker.platform.pickFiles(
        type: FileType.video,
        allowMultiple: false,
        withData: true,
      );
      if (result != null && result.files.isNotEmpty) {
        final file = result.files.first;
        if (file.bytes != null && mounted) {
          Navigator.push(
            context,
            MaterialPageRoute(
              builder: (_) => VideoPlayerPage(
                videoBytes: file.bytes,
                videoName: file.name,
              ),
            ),
          );
        }
      }
    } else {
      final result = await FilePicker.platform.pickFiles(type: FileType.video);
      if (result != null && result.files.isNotEmpty) {
        final path = result.files.first.path;
        if (path != null && mounted) {
          await _saveLastVideo(path, 'local');
          Navigator.push(
            context,
            MaterialPageRoute(
              builder: (_) => VideoPlayerPage(localPath: path),
            ),
          );
        }
      }
    }
  }

  void _selectVideo(String path, String type) {
    _saveLastVideo(path, type);
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (_) => type == 'asset' 
            ? VideoPlayerPage(assetPath: path)
            : VideoPlayerPage(localPath: path),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Expanded(
          child: ListView.builder(
            itemCount: _videos.length,
            itemBuilder: (ctx, i) {
              final v = _videos[i];
              final isSelected = v['path'] == _selectedPath && v['type'] == _selectedType;
              return ListTile(
                leading: Icon(
                  isSelected ? Icons.play_circle_filled : Icons.video_library,
                  color: isSelected ? Colors.red : Colors.white,
                  size: 40,
                ),
                title: Text(
                  v['name']!,
                  style: const TextStyle(color: Colors.white, fontSize: 18),
                ),
                subtitle: Text(
                  v['type'] == 'asset' ? 'Bundled Asset' : 'Local File',
                  style: const TextStyle(color: Colors.white70),
                ),
                onTap: () => _selectVideo(v['path']!, v['type']!),
                selected: isSelected,
                selectedTileColor: Colors.red.withValues(alpha: 0.2),
              );
            },
          ),
        ),
        Container(
          width: double.infinity,
          padding: const EdgeInsets.all(16),
          child: ElevatedButton.icon(
            onPressed: _pickLocalFile,
            icon: const Icon(Icons.folder_open),
            label: Text(kIsWeb ? 'Pick File from Device' : 'Pick Local File'),
            style: ElevatedButton.styleFrom(
              backgroundColor: Colors.red,
              foregroundColor: Colors.white,
              padding: const EdgeInsets.symmetric(vertical: 16),
            ),
          ),
        ),
      ],
    );
  }
}

class VideoPlayerPage extends StatefulWidget {
  final String? assetPath;
  final String? localPath;
  final Uint8List? videoBytes;
  final String? videoName;

  const VideoPlayerPage({
    super.key,
    this.assetPath,
    this.localPath,
    this.videoBytes,
    this.videoName,
  });

  @override
  State<VideoPlayerPage> createState() => _VideoPlayerPageState();
}

class _VideoPlayerPageState extends State<VideoPlayerPage> {
  VideoPlayerController? _controller;
  bool _isInitialized = false;
  bool _isLoading = true;
  bool _showControls = true;
  bool _isBuffering = false;
  String? _errorMessage;
  String? _blobUrl;

  @override
  void initState() {
    super.initState();
    _initVideo();
  }

  Future<void> _initVideo() async {
    setState(() {
      _isLoading = true;
      _errorMessage = null;
    });

    try {
      if (widget.videoBytes != null && kIsWeb) {
        _blobUrl = _JsHelpers.makeUrl(widget.videoBytes!);
        _controller = VideoPlayerController.networkUrl(Uri.parse(_blobUrl!));
      } else if (widget.localPath != null) {
        if (kIsWeb) {
          _errorMessage = 'Local files not supported on web. Use file picker.';
          setState(() => _isLoading = false);
          return;
        }
        _controller = VideoPlayerController.file(File(widget.localPath!));
      } else if (widget.assetPath != null) {
        _controller = VideoPlayerController.asset(widget.assetPath!);
      } else {
        throw Exception('No video source');
      }

      _controller!.addListener(_onControllerUpdate);
      await _controller!.initialize();
      if (mounted) {
        setState(() {
          _isInitialized = true;
          _isLoading = false;
        });
      }
    } catch (e) {
      if (mounted) {
        setState(() {
          _errorMessage = 'Error: $e';
          _isLoading = false;
        });
      }
    }
  }

  @override
  void dispose() {
    _controller?.removeListener(_onControllerUpdate);
    if (_blobUrl != null && kIsWeb) {
      try { _JsHelpers.revokeUrl(_blobUrl!); } catch (_) {}
    }
    _controller?.dispose();
    super.dispose();
  }

  void _onControllerUpdate() {
    if (_controller == null) return;
    final buffering = _controller!.value.isBuffering;
    if (buffering != _isBuffering) {
      _isBuffering = buffering;
    }
    if (mounted) setState(() {});
  }

  void _togglePlayPause() {
    if (_controller == null) return;
    setState(() {
      if (_controller!.value.isPlaying) {
        _controller!.pause();
      } else {
        _controller!.play();
      }
    });
  }

  void _seek(Duration delta) {
    if (_controller == null) return;
    final pos = _controller!.value.position;
    final dur = _controller!.value.duration;
    final newPos = pos + delta;
    _controller!.seekTo(newPos.isNegative ? Duration.zero : (newPos > dur ? dur : newPos));
    setState(() {});  // Force rebuild to update slider
  }

  String _fmt(Duration d) {
    String td(int n) => n.toString().padLeft(2, '0');
    final h = d.inHours;
    final m = d.inMinutes.remainder(60);
    final s = d.inSeconds.remainder(60);
    if (h > 0) return '${td(h)}:${td(m)}:${td(s)}';
    return '${td(m)}:${td(s)}';
  }

  void _onSeek(double v) {
    if (_controller == null) return;
    final dur = _controller!.value.duration;
    _controller!.seekTo(Duration(milliseconds: (dur.inMilliseconds * v).round()));
    setState(() {});  // Force rebuild to update time display
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.videoName ?? 'Video'),
        backgroundColor: Colors.black87,
        foregroundColor: Colors.white,
      ),
      backgroundColor: Colors.black,
      body: Center(
        child: _isLoading
            ? const CircularProgressIndicator(color: Colors.white)
            : _errorMessage != null
                ? _buildError()
                : _isInitialized
                    ? _buildPlayer()
                    : const CircularProgressIndicator(color: Colors.white),
      ),
    );
  }

  Widget _buildError() => Column(
    mainAxisAlignment: MainAxisAlignment.center,
    children: [
      const Icon(Icons.error, color: Colors.red, size: 48),
      const SizedBox(height: 16),
      Padding(
        padding: const EdgeInsets.all(16),
        child: Text(_errorMessage!, style: const TextStyle(color: Colors.white)),
      ),
      ElevatedButton(
        onPressed: () => Navigator.pop(context),
        child: const Text('Go Back'),
      ),
    ],
  );

  Widget _buildPlayer() {
    final c = _controller!;
    final v = c.value;
    return GestureDetector(
      onTap: () => setState(() => _showControls = !_showControls),
      child: Stack(
        fit: StackFit.expand,
        children: [
          Center(child: AspectRatio(aspectRatio: v.aspectRatio, child: VideoPlayer(c))),
          if (_isBuffering) const Center(child: CircularProgressIndicator(color: Colors.red)),
          if (_showControls) _buildControls(v),
        ],
      ),
    );
  }

  Widget _buildControls(VideoPlayerValue v) => Container(
    color: Colors.black54,
    child: Column(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [
        Row(mainAxisAlignment: MainAxisAlignment.center, children: [
          IconButton(
            icon: const Icon(Icons.replay_10, color: Colors.white, size: 32),
            onPressed: () => _seek(const Duration(seconds: -10)),
          ),
          IconButton(
            icon: Icon(v.isPlaying ? Icons.pause : Icons.play_arrow, color: Colors.white, size: 48),
            onPressed: _togglePlayPause,
          ),
          IconButton(
            icon: const Icon(Icons.forward_10, color: Colors.white, size: 32),
            onPressed: () => _seek(const Duration(seconds: 10)),
          ),
        ]),
        Padding(
          padding: const EdgeInsets.symmetric(horizontal: 16),
          child: Row(children: [
            Text(_fmt(v.position), style: const TextStyle(color: Colors.white, fontSize: 12)),
            Expanded(child: Slider(
              value: (v.position.inMilliseconds / v.duration.inMilliseconds).clamp(0.0, 1.0),
              onChanged: _onSeek,
              activeColor: Colors.red,
            )),
            Text(_fmt(v.duration), style: const TextStyle(color: Colors.white, fontSize: 12)),
          ]),
        ),
        const SizedBox(height: 16),
      ],
    ),
  );
}
