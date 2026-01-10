/**
 * 搜索页面组件
 */

import React, { useState } from 'react';
import { searchAPI, Track, playlistAPI } from '../services/api';

export const SearchPage: React.FC = () => {
  const [query, setQuery] = useState('');
  const [results, setResults] = useState<Track[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  // 执行搜索
  const handleSearch = async (e: React.FormEvent) => {
    e.preventDefault();

    if (!query.trim()) {
      setResults([]);
      return;
    }

    setLoading(true);
    try {
      const data = await searchAPI.search(query);
      setResults(data);
      setError(null);
    } catch (err) {
      setError(`搜索失败: ${err instanceof Error ? err.message : '未知错误'}`);
    } finally {
      setLoading(false);
    }
  };

  // 添加到播放列表
  const handleAddTrack = async (track: Track) => {
    try {
      await playlistAPI.addTrack(track);
      alert(`已添加: ${track.title}`);
      setError(null);
    } catch (err) {
      setError(`添加失败: ${err instanceof Error ? err.message : '未知错误'}`);
    }
  };

  return (
    <div className="search-page">
      <div className="search-header">
        <form onSubmit={handleSearch} className="search-form">
          <input
            type="text"
            placeholder="搜索音乐..."
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            className="search-input"
          />
          <button type="submit" disabled={loading} className="search-btn">
            {loading ? '搜索中...' : '搜索'}
          </button>
        </form>
      </div>

      {error && <div className="search-error">{error}</div>}

      <div className="search-results">
        {results.length === 0 ? (
          <div className="search-empty">
            {query ? '未找到搜索结果' : '输入关键词开始搜索'}
          </div>
        ) : (
          <>
            <div className="results-count">找到 {results.length} 个结果</div>
            <div className="results-list">
              {results.map((track) => (
                <div key={track.id} className="result-item">
                  {track.coverUrl && (
                    <img src={track.coverUrl} alt={track.title} className="result-cover" />
                  )}
                  <div className="result-info">
                    <h4>{track.title}</h4>
                    <p>{track.artist} - {track.album}</p>
                    <p className="result-source">来源: {track.source}</p>
                  </div>
                  <div className="result-actions">
                    <button onClick={() => handleAddTrack(track)} className="btn-add">
                      添加到列表
                    </button>
                  </div>
                </div>
              ))}
            </div>
          </>
        )}
      </div>
    </div>
  );
};
