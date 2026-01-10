/**
 * 后端 API 客户端
 * 所有前端请求都通过这个服务进行
 */

const API_BASE_URL = 'http://127.0.0.1:8888/api';

export interface Track {
  id: string;
  title: string;
  artist: string;
  album: string;
  url: string;
  duration: number;
  source: string;
  coverUrl?: string;
}

export interface Playlist {
  id: string;
  name: string;
  tracks: Track[];
  createdAt: number;
  updatedAt: number;
}

export interface PlayerStatus {
  state: 'playing' | 'paused' | 'stopped';
  position: number;
  volume: number;
  duration: number;
  currentTrack?: Track;
}

// ===== 错误处理 =====

class ApiError extends Error {
  constructor(
    public code: number,
    public message: string,
    public details?: any
  ) {
    super(message);
    this.name = 'ApiError';
  }
}

async function handleResponse(response: Response) {
  if (!response.ok) {
    const error = await response.json().catch(() => ({}));
    throw new ApiError(
      response.status,
      error.message || response.statusText,
      error
    );
  }

  const contentType = response.headers.get('content-type');
  if (contentType?.includes('application/json')) {
    return response.json();
  }

  return response.text();
}

// ===== 播放器 API =====

export const playerAPI = {
  /**
   * 加载音乐文件
   * @param filePath 文件路径或URL
   */
  async load(filePath: string): Promise<Track> {
    const response = await fetch(`${API_BASE_URL}/player/load`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ filePath })
    });
    return handleResponse(response);
  },

  /**
   * 播放
   */
  async play(): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/player/play`, {
      method: 'POST'
    });
    await handleResponse(response);
  },

  /**
   * 暂停
   */
  async pause(): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/player/pause`, {
      method: 'POST'
    });
    await handleResponse(response);
  },

  /**
   * 停止
   */
  async stop(): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/player/stop`, {
      method: 'POST'
    });
    await handleResponse(response);
  },

  /**
   * 跳转到指定位置
   * @param position 位置（毫秒）
   */
  async seek(position: number): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/player/seek`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ position })
    });
    await handleResponse(response);
  },

  /**
   * 设置音量
   * @param volume 音量等级 0-100
   */
  async setVolume(volume: number): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/player/volume`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ volume })
    });
    await handleResponse(response);
  },

  /**
   * 获取播放器状态
   */
  async getStatus(): Promise<PlayerStatus> {
    const response = await fetch(`${API_BASE_URL}/player/status`);
    return handleResponse(response);
  }
};

// ===== 播放列表 API =====

export const playlistAPI = {
  /**
   * 获取当前播放列表
   */
  async get(): Promise<Playlist> {
    const response = await fetch(`${API_BASE_URL}/playlist`);
    return handleResponse(response);
  },

  /**
   * 添加轨道到播放列表
   * @param track 轨道信息
   */
  async addTrack(track: Track): Promise<Playlist> {
    const response = await fetch(`${API_BASE_URL}/playlist/add`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(track)
    });
    return handleResponse(response);
  },

  /**
   * 删除播放列表中的轨道
   * @param index 轨道索引
   */
  async removeTrack(index: number): Promise<Playlist> {
    const response = await fetch(`${API_BASE_URL}/playlist/${index}`, {
      method: 'DELETE'
    });
    return handleResponse(response);
  },

  /**
   * 清空播放列表
   */
  async clear(): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/playlist/clear`, {
      method: 'POST'
    });
    await handleResponse(response);
  },

  /**
   * 播放下一首
   */
  async playNext(): Promise<Track> {
    const response = await fetch(`${API_BASE_URL}/playlist/next`);
    return handleResponse(response);
  },

  /**
   * 播放上一首
   */
  async playPrevious(): Promise<Track> {
    const response = await fetch(`${API_BASE_URL}/playlist/prev`);
    return handleResponse(response);
  }
};

// ===== 搜索和发现 API =====

export const searchAPI = {
  /**
   * 搜索音乐
   * @param query 搜索关键词
   * @param limit 返回结果数量
   */
  async search(query: string, limit: number = 20): Promise<Track[]> {
    const params = new URLSearchParams({ q: query, limit: limit.toString() });
    const response = await fetch(`${API_BASE_URL}/search?${params}`);
    return handleResponse(response);
  }
};

// ===== 用户数据 API =====

export const userAPI = {
  /**
   * 获取收藏列表
   */
  async getFavorites(): Promise<Track[]> {
    const response = await fetch(`${API_BASE_URL}/favorites`);
    return handleResponse(response);
  },

  /**
   * 添加到收藏
   * @param track 轨道信息
   */
  async addFavorite(track: Track): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/favorites`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(track)
    });
    await handleResponse(response);
  },

  /**
   * 从收藏删除
   * @param trackId 轨道ID
   */
  async removeFavorite(trackId: string): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/favorites/${trackId}`, {
      method: 'DELETE'
    });
    await handleResponse(response);
  },

  /**
   * 获取播放历史
   * @param limit 返回最近N条记录
   */
  async getHistory(limit: number = 100): Promise<Track[]> {
    const response = await fetch(`${API_BASE_URL}/history?limit=${limit}`);
    return handleResponse(response);
  },

  /**
   * 清空播放历史
   */
  async clearHistory(): Promise<void> {
    const response = await fetch(`${API_BASE_URL}/history`, {
      method: 'DELETE'
    });
    await handleResponse(response);
  }
};

// ===== 系统 API =====

export const systemAPI = {
  /**
   * 获取已加载的插件
   */
  async getPlugins(): Promise<string[]> {
    const response = await fetch(`${API_BASE_URL}/plugins`);
    return handleResponse(response);
  },

  /**
   * 健康检查
   */
  async healthCheck(): Promise<{ status: 'ok'; version: string }> {
    const response = await fetch(`${API_BASE_URL}/health`);
    return handleResponse(response);
  }
};

export default {
  playerAPI,
  playlistAPI,
  searchAPI,
  userAPI,
  systemAPI,
  ApiError
};
