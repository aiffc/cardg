#pragma once

#include <cstdint>

namespace cg::engine {

class Time final {
  private:
    uint64_t m_last_frame_time{0};
    uint64_t m_start_frame_time{0};
    double m_delta_time{0.0};
    uint32_t m_fps{0};
    double m_frame_interval{0.0};

  private:
    /**
     * @brief
     * delay some time, make sure fps work currectly
     *
     * @param [l2s_interval] delay some time
     */
    void limit(uint64_t l2s_interval);

  public:
    Time(uint32_t fps = 144);
    ~Time();
    /**
     * @brief
     * time init function update start time & last time
     */
    void init();
    /**
     * @brief
     * nothing just log
     */
    void deinit();
    /**
     * @brief
     * set fps
     *
     * @param [fps] the except fps
     */
    void setfps(uint32_t fps);
    /**
     * @brief
     * update start time & last time, if fps setted delay some time
     */
    void update();
    /**
     * @brief
     * deleta time getter function
     *
     * @return float
     * delta time
     */
    float getDeltaTime() const { return static_cast<float>(m_delta_time); }
    /**
     * @brief
     * fps getter function
     *
     * @return uint32_t
     * current fps setted
     */
    uint32_t getfps() const { return m_fps; }

    Time(Time &) = delete;
    Time(Time &&) = delete;
    Time &operator=(Time &) = delete;
    Time &operator=(Time &&) = delete;
};

} // namespace cg::engine
