#include "util/xinput_source.h"
#include "util/input_manager.h"

#include "common/assert.h"
#include "common/log.h"
#include "common/string_util.h"

#include "IconsPromptFont.h"

#include <cmath>

XInputSource::XInputSource() = default;

XInputSource::~XInputSource() = default;

bool XInputSource::Initialize(SettingsInterface& si, std::unique_lock<std::mutex>& settings_lock)
{
  return true;
}

void XInputSource::UpdateSettings(SettingsInterface& si, std::unique_lock<std::mutex>& settings_lock)
{
}

bool XInputSource::ReloadDevices()
{
  return false;
}

void XInputSource::Shutdown()
{
}

void XInputSource::PollEvents()
{
}

std::vector<std::pair<std::string, std::string>> XInputSource::EnumerateDevices()
{
  std::vector<std::pair<std::string, std::string>> ret;

  return ret;
}

std::optional<InputBindingKey> XInputSource::ParseKeyString(const std::string_view& device,
                                                            const std::string_view& binding)
{
  return std::nullopt;
}

TinyString XInputSource::ConvertKeyToString(InputBindingKey key)
{
  TinyString ret;

  return ret;
}

TinyString XInputSource::ConvertKeyToIcon(InputBindingKey key)
{
  TinyString ret;

  return ret;
}

std::vector<InputBindingKey> XInputSource::EnumerateMotors()
{
  std::vector<InputBindingKey> ret;

  return ret;
}

bool XInputSource::GetGenericBindingMapping(const std::string_view& device, GenericInputBindingMapping* mapping)
{
  return false;
}

void XInputSource::HandleControllerConnection(u32 index)
{
}

void XInputSource::HandleControllerDisconnection(u32 index)
{
}

void XInputSource::CheckForStateChanges(u32 index, const XINPUT_STATE& new_state)
{
}

void XInputSource::UpdateMotorState(InputBindingKey key, float intensity)
{
}

void XInputSource::UpdateMotorState(InputBindingKey large_key, InputBindingKey small_key, float large_intensity,
                                    float small_intensity)
{
}

std::unique_ptr<InputSource> InputSource::CreateXInputSource()
{
  return std::make_unique<XInputSource>();
}
