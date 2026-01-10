/**
 * 播放列表组件
 */

import React, { useState, useEffect } from 'react';
import { playlistAPI, Playlist } from '../services/api';

export const PlaylistComponent: React.FC = () => {
  const [playlist, setPlaylist] = useState<Playlist | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  // 获取播放列表
  const loadPlaylist = async () => {
    setLoading(true);
    try {
      const data = await playlistAPI.get();
      setPlaylist(data);
      setError(null);
    } catch (err) {
      setError(`加载失败: ${err instanceof Error ? err.message : '未知错误'}`);
    } finally {
      setLoading(false);
    }
  };

  // 初始化时加载
  useEffect(() => {
    loadPlaylist();
  }, []);

  // 删除轨道
  const handleRemoveTrack = async (index: number) => {
    try {
      const data = await playlistAPI.removeTrack(index);
      setPlaylist(data);
      setError(null);
    } catch (err) {
      setError(`删除失败: ${err instanceof Error ? err.message : '未知错误'}`);
    }
  };

  // 清空列表
  const handleClear = async () => {
    if (!window.confirm('确定要清空播放列表吗？')) return;
    try {
      await playlistAPI.clear();
      setPlaylist(null);
      setError(null);
    } catch (err) {
      setError(`清空失败: ${err instanceof Error ? err.message : '未知错误'}`);
    }
  };

  if (loading) {
    return <div className="playlist playlist-loading">加载中...</div>;
  }

  if (!playlist || playlist.tracks.length === 0) {
    return (
      <div className="playlist playlist-empty">
        <p>播放列表为空</p>
        <button onClick={loadPlaylist}>刷新</button>
      </div>
    );
  }

  return (
    <div className="playlist">
      <div className="playlist-header">
        <h2>{playlist.name}</h2>
        <span className="playlist-count">共 {playlist.tracks.length} 首</span>
        <button onClick={handleClear} className="btn-clear">
          清空
        </button>
      </div>

      {error && <div className="playlist-error">{error}</div>}

      <div className="playlist-body">
        <table className="playlist-table">
          <thead>
            <tr>
              <th>序号</th>
              <th>标题</th>
              <th>艺术家</th>
              <th>专辑</th>
              <th>时长</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            {playlist.tracks.map((track, index) => (
              <tr key={track.id} className="playlist-item">
                <td>{index + 1}</td>
                <td>{track.title}</td>
                <td>{track.artist}</td>
                <td>{track.album}</td>
                <td>{formatDuration(track.duration)}</td>
                <td>
                  <button
                    onClick={() => handleRemoveTrack(index)}
                    className="btn-remove"
                  >
                    删除
                  </button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

// 时长格式化
function formatDuration(ms: number): string {
  const totalSeconds = Math.floor(ms / 1000);
  const minutes = Math.floor(totalSeconds / 60);
  const seconds = totalSeconds % 60;
  return `${minutes}:${seconds.toString().padStart(2, '0')}`;
}
