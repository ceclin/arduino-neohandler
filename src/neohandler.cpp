#include "neohandler.h"

bool neohandler::BaseHandler::UpdateMillis()
{
    unsigned long temp = millis();
    if (temp - this->millis_ >= this->interval())
    {
        this->millis_ = temp;
        return true;
    }
    else
        return false;
}

bool neohandler::BaseHandler::Update()
{
    return this->CurrentState() == neohandler::State::RUNNING && this->UpdateMillis();
}

neohandler::State neohandler::BaseHandler::CurrentState()
{
    using neohandler::State;
    if (this->state_ & B00000001)
        return State::RUNNING;
    else
        return State::PAUSED;
}

neohandler::LoopHandler::LoopHandler(unsigned long interval) : BaseHandler{interval}
{
    this->Resume();
}

void neohandler::CallbackLoopHandler::CallIfUpdated()
{
    if (this->Update())
    {
        this->InvokeCallback();
    }
}

neohandler::RepeatHandler::RepeatHandler(unsigned long interval, uint8_t times) : BaseHandler{interval}
{
    this->set_times(times);
}

void neohandler::RepeatHandler::set_times(uint8_t times)
{
    this->times_ = times;
    if (times == 0)
        this->Pause();
    else // times > 0
        this->Resume();
}

bool neohandler::RepeatHandler::Update()
{
    if (this->BaseHandler::Update())
    {
        this->set_times(this->times() - 1);
        return true;
    }
    return false;
}

void neohandler::CallbackRepeatHandler::CallIfUpdated()
{
    if (this->RepeatHandler::Update())
    {
        this->InvokeCallback();
    }
}
