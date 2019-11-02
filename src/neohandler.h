#ifndef NEOHANDLER_H_
#define NEOHANDLER_H_
#include <Arduino.h>
namespace neohandler
{
enum State : uint8_t
{
    PAUSED,
    RUNNING,
};

class CallbackHandler
{
private:
    void (*callback_)();

protected:
    explicit CallbackHandler(void (*callback)()) : callback_{callback} {}
    inline void InvokeCallback()
    {
        if (callback_ != nullptr)
            callback_();
    }

public:
    inline void (*callback())()
    {
        return this->callback_;
    }
    inline void set_callback(void (*callback)())
    {
        this->callback_ = callback;
    }
    virtual void CallIfUpdated() = 0;
};

class BaseHandler
{
private:
    unsigned long millis_;
    unsigned long interval_;
    bool UpdateMillis();

protected:
    uint8_t state_ = B00000000;
    explicit BaseHandler(unsigned long interval = 0) : millis_{millis()}, interval_{interval} {}

public:
    inline unsigned long interval()
    {
        return this->interval_;
    }
    inline void set_interval(unsigned long interval)
    {
        this->interval_ = interval;
    }
    inline void Pause()
    {
        this->state_ &= B11111110;
    }
    inline void Resume()
    {
        this->state_ |= B00000001;
    }
    inline void RefreshTime()
    {
        this->millis_ = millis();
    }
    virtual bool Update();
    State CurrentState();
};

class LoopHandler : public BaseHandler
{
private:
    unsigned long interval_;

public:
    explicit LoopHandler(unsigned long interval = 0);
};

class CallbackLoopHandler : public LoopHandler, public CallbackHandler
{
public:
    CallbackLoopHandler(unsigned long interval = 0, void (*callback)() = nullptr)
        : LoopHandler{interval}, CallbackHandler{callback} {}
    void CallIfUpdated() override;
};

class RepeatHandler : public BaseHandler
{
private:
    unsigned long interval_;
    /**
     * If times_ is modified, you must set the corresponding state.
     */
    uint8_t times_;

public:
    RepeatHandler(unsigned long interval = 0, uint8_t times = 0);
    inline uint8_t times()
    {
        return this->times_;
    }
    void set_times(uint8_t times);
    bool Update() override;
};

class CallbackRepeatHandler : public RepeatHandler, public CallbackHandler
{
public:
    CallbackRepeatHandler(unsigned long interval = 0, uint8_t times = 0, void (*callback)() = nullptr)
        : RepeatHandler{interval, times}, CallbackHandler{callback} {}
    void CallIfUpdated() override;
};
} // namespace neohandler
#endif // NEOHANDLER_H_
