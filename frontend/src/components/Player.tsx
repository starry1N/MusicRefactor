/**
 * 播放器组件
 * 主要的音乐播放控制界面
 */

import React, { useState, useEffect } from 'react';
import { playerAPI, PlayerStatus } from '../services/api';

export const Player: React.FC = () => {
  const [status, setStatus] = useState<PlayerStatus | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  // 获取播放器状态
  const updateStatus = async () => {
    try {
      const newStatus = await playerAPI.getStatus();
      setStatus(newStatus);
      setError(null);
    } catch (err) {
      setError(`获取状态失败: ${err instanceof Error ? err.message : '未知错误'}`);
    }
  };

  // 初始化时获取状态
  useEffect(() => {
    updateStatus();
    // 每秒更新一次播放位置
    const interval = setInterval(updateStatus, 1000);
    return () => clearInterval(interval);
  }, []);

  // 播放
  const handlePlay = async () => {
    setLoading(true);
    try {
      await playerAPI.play();
      await updateStatus();
      setError(null);
    } catch (err) {
      setError(`播放失败: ${err instanceof Error ? err.message : '未知错误'}`);
    } finally {
      setLoading(false);
    }
  };

  // 暂停
  const handlePause = async () => {
    setLoading(true);
    try {
      await playerAPI.pause();
      await updateStatus();
      setError(null);
    } catch (err) {
      setError(`暂停失败: ${err instanceof Error ? err.message : '未知错误'}`);
    } finally {
      setLoading(false);
    }
  };

  // 停止
  const handleStop = async () => {
    setLoading(true);
    try {
      await playerAPI.stop();
      await updateStatus();
      setError(null);
    } catch (err) {
      setError(`停止失败: ${err instanceof Error ? err.message : '未知错误'}`);
    } finally {
      setLoading(false);
    }
  };

  // 设置音量
  const handleVolumeChange = async (volume: number) => {
    try {
      await playerAPI.setVolume(volume);
      await updateStatus();
      setError(null);
    } catch (err) {
      setError(`设置音量失败: ${err instanceof Error ? err.message : '未知错误'}`);
    }
  };

  // 跳转
  const handleSeek = async (position: number) => {
    try {
      await playerAPI.seek(position);
      await updateStatus();
      setError(null);
    } catch (err) {
      setError(`跳转失败: ${err instanceof Error ? err.message : '未知错误'}`);
    }
  };

  if (!status) {
    return <div className="player player-loading">加载中...</div>;
  }

  const duration = status.duration || 0;
  const position = status.position || 0;
  const volume = status.volume || 50;
  const progress = duration > 0 ? (position / duration) * 100 : 0;

  return (
    <div className="player">
      <div className="player-info">
        {status.currentTrack ? (
          <>
            <h3>{status.currentTrack.title}</h3>
            <p>{status.currentTrack.artist}</p>
          </>
        ) : (
          <p>未加载音乐</p>
        )}
      </div>

      {error && <div className="player-error">{error}</div>}

      <div className="player-progress">
        <div
          className="player-progress-bar"
          style={{ width: `${progress}%` }}
          onClick={(e) => {
            const rect = e.currentTarget.parentElement!.getBoundingClientRect();
            const newPosition = (e.clientX - rect.left) / rect.width * duration;
            handleSeek(newPosition);
          }}
        />
        <input
          type="range"
          min="0"
          max={duration}
          value={position}
          onChange={(e) => handleSeek(Number(e.target.value))}
          className="player-progress-input"
        />
      </div>

      <div className="player-time">
        <span>{formatTime(position)}</span>
        <span>{formatTime(duration)}</span>
      </div>

      <div className="player-controls">
        <button onClick={handleStop} disabled={loading}>
          ⏹ 停止
        </button>
        <button onClick={handlePlay} disabled={loading}>
          ▶ 播放
        </button>
        <button onClick={handlePause} disabled={loading}>
          ⏸ 暂停
        </button>
      </div>

      <div className="player-volume">
        <label>音量</label>
        <input
          type="range"
          min="0"
          max="100"
          value={volume}
          onChange={(e) => handleVolumeChange(Number(e.target.value))}
        />
        <span>{volume}%</span>
      </div>
    </div>
  );
};

// 时间格式化
function formatTime(ms: number): string {
  const totalSeconds = Math.floor(ms / 1000);
  const minutes = Math.floor(totalSeconds / 60);
  const seconds = totalSeconds % 60;
  return `${minutes}:${seconds.toString().padStart(2, '0')}`;
}
