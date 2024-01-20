#pragma once

#include "EventHandler.hxx"

class EventHandlerCore : public EventHandler {
	public:
		explicit EventHandlerCore(OSystem& osystem) : EventHandler(osystem) {}
		~EventHandlerCore() override = default;

	private:
		void enableTextEvents(bool enable) override {}
		void pollEvent() override {}

	private:
		EventHandlerCore() = delete;
		EventHandlerCore(const EventHandlerCore&) = delete;
		EventHandlerCore(EventHandlerCore&&) = delete;
		EventHandlerCore& operator=(const EventHandlerCore&) = delete;
		EventHandlerCore& operator=(EventHandlerCore&&) = delete;
};
