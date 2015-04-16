//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group120.h"

#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

// ------- Group120Var1 -------

Group120Var1::Group120Var1() : 
  challengeSeqNum(0), userNum(0), macAlgo(HMACType::UNKNOWN), challengeReason(ChallengeReason::UNKNOWN)
{}

Group120Var1::Group120Var1(
  uint32_t challengeSeqNum_,
  uint16_t userNum_,
  HMACType macAlgo_,
  ChallengeReason challengeReason_,
  const openpal::ReadBufferView& challengeData_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  macAlgo(macAlgo_),
  challengeReason(challengeReason_),
  challengeData(challengeData_)
{}

uint32_t Group120Var1::Size() const
{
  return MIN_SIZE + challengeData.Size();
}

bool Group120Var1::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var1::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->challengeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);
  this->macAlgo = HMACTypeFromType(UInt8::ReadBuffer(copy));
  this->challengeReason = ChallengeReasonFromType(UInt8::ReadBuffer(copy));

  this->challengeData = copy; // whatever is left over

  return true;
}

bool Group120Var1::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->challengeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);
  UInt8::WriteBuffer(buffer, HMACTypeToType(this->macAlgo));
  UInt8::WriteBuffer(buffer, ChallengeReasonToType(this->challengeReason));

  challengeData.CopyTo(buffer);

  return true;
}

// ------- Group120Var2 -------

Group120Var2::Group120Var2() : 
  challengeSeqNum(0), userNum(0)
{}

Group120Var2::Group120Var2(
  uint32_t challengeSeqNum_,
  uint16_t userNum_,
  const openpal::ReadBufferView& hmac_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  hmac(hmac_)
{}

uint32_t Group120Var2::Size() const
{
  return MIN_SIZE + hmac.Size();
}

bool Group120Var2::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var2::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->challengeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);

  this->hmac = copy; // whatever is left over

  return true;
}

bool Group120Var2::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->challengeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);

  hmac.CopyTo(buffer);

  return true;
}

// ------- Group120Var3 -------

bool Group120Var3::Read(ReadBufferView& buffer, Group120Var3& output)
{
  return Parse::Many(buffer, output.challengeSeqNum, output.userNum);
}

bool Group120Var3::Write(const Group120Var3& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.challengeSeqNum, arg.userNum);
}

// ------- Group120Var4 -------

bool Group120Var4::Read(ReadBufferView& buffer, Group120Var4& output)
{
  return Parse::Many(buffer, output.userNum);
}

bool Group120Var4::Write(const Group120Var4& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.userNum);
}

// ------- Group120Var5 -------

Group120Var5::Group120Var5() : 
  keyChangeSeqNum(0), userNum(0), keyWrapAlgo(KeyWrapAlgorithm::UNDEFINED), keyStatus(KeyStatus::UNDEFINED), macAlgo(HMACType::UNKNOWN)
{}

Group120Var5::Group120Var5(
  uint32_t keyChangeSeqNum_,
  uint16_t userNum_,
  KeyWrapAlgorithm keyWrapAlgo_,
  KeyStatus keyStatus_,
  HMACType macAlgo_,
  const openpal::ReadBufferView& challengeData_,
  const openpal::ReadBufferView& hmac_
) : 
  keyChangeSeqNum(keyChangeSeqNum_),
  userNum(userNum_),
  keyWrapAlgo(keyWrapAlgo_),
  keyStatus(keyStatus_),
  macAlgo(macAlgo_),
  challengeData(challengeData_),
  hmac(hmac_)
{}

uint32_t Group120Var5::Size() const
{
  return MIN_SIZE + challengeData.Size() + hmac.Size();
}

bool Group120Var5::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var5::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->keyChangeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);
  this->keyWrapAlgo = KeyWrapAlgorithmFromType(UInt8::ReadBuffer(copy));
  this->keyStatus = KeyStatusFromType(UInt8::ReadBuffer(copy));
  this->macAlgo = HMACTypeFromType(UInt8::ReadBuffer(copy));

  if(!IVariableLength::ReadUInt16PrefixedField(copy, this->challengeData))
  {
    return false;
  }

  this->hmac = copy; // whatever is left over

  return true;
}

bool Group120Var5::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  if(challengeData.Size() > openpal::MaxValue<uint16_t>())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->keyChangeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);
  UInt8::WriteBuffer(buffer, KeyWrapAlgorithmToType(this->keyWrapAlgo));
  UInt8::WriteBuffer(buffer, KeyStatusToType(this->keyStatus));
  UInt8::WriteBuffer(buffer, HMACTypeToType(this->macAlgo));

  UInt16::WriteBuffer(buffer, static_cast<uint16_t>(challengeData.Size()));
  challengeData.CopyTo(buffer);

  hmac.CopyTo(buffer);

  return true;
}

// ------- Group120Var6 -------

Group120Var6::Group120Var6() : 
  keyChangeSeqNum(0), userNum(0)
{}

Group120Var6::Group120Var6(
  uint32_t keyChangeSeqNum_,
  uint16_t userNum_,
  const openpal::ReadBufferView& keyWrapData_
) : 
  keyChangeSeqNum(keyChangeSeqNum_),
  userNum(userNum_),
  keyWrapData(keyWrapData_)
{}

uint32_t Group120Var6::Size() const
{
  return MIN_SIZE + keyWrapData.Size();
}

bool Group120Var6::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var6::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->keyChangeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);

  this->keyWrapData = copy; // whatever is left over

  return true;
}

bool Group120Var6::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->keyChangeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);

  keyWrapData.CopyTo(buffer);

  return true;
}

// ------- Group120Var7 -------

Group120Var7::Group120Var7() : 
  challengeSeqNum(0), userNum(0), assocId(0), errorCode(AuthErrorCode::UNKNOWN), time(0)
{}

Group120Var7::Group120Var7(
  uint32_t challengeSeqNum_,
  uint16_t userNum_,
  uint16_t assocId_,
  AuthErrorCode errorCode_,
  DNPTime time_,
  const openpal::ReadBufferView& errorText_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  assocId(assocId_),
  errorCode(errorCode_),
  time(time_),
  errorText(errorText_)
{}

uint32_t Group120Var7::Size() const
{
  return MIN_SIZE + errorText.Size();
}

bool Group120Var7::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var7::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->challengeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);
  this->assocId = UInt16::ReadBuffer(copy);
  this->errorCode = AuthErrorCodeFromType(UInt8::ReadBuffer(copy));
  this->time = UInt48::ReadBuffer(copy);

  this->errorText = copy; // whatever is left over

  return true;
}

bool Group120Var7::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->challengeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);
  UInt16::WriteBuffer(buffer, this->assocId);
  UInt8::WriteBuffer(buffer, AuthErrorCodeToType(this->errorCode));
  UInt48::WriteBuffer(buffer, this->time);

  errorText.CopyTo(buffer);

  return true;
}

// ------- Group120Var9 -------

Group120Var9::Group120Var9()
{}

Group120Var9::Group120Var9(
  const openpal::ReadBufferView& hmac_
) : 
  hmac(hmac_)
{}

uint32_t Group120Var9::Size() const
{
  return MIN_SIZE + hmac.Size();
}

bool Group120Var9::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var9::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->hmac = copy; // whatever is left over

  return true;
}

bool Group120Var9::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  hmac.CopyTo(buffer);

  return true;
}


}
