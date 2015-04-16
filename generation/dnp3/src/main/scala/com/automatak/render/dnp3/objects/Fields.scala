package com.automatak.render.dnp3.objects

import com.automatak.render.EnumModel
import com.automatak.render.dnp3.enums._
import com.automatak.render.dnp3.objects.generators.FixedSizeHelpers

sealed trait FieldType
sealed class FixedSizeFieldType(val numBytes: Int) extends FieldType {
  def defaultValue: String = "0"
}

case object UInt8Field extends FixedSizeFieldType(1)
case object UInt16Field extends FixedSizeFieldType(2)
case object UInt32Field extends FixedSizeFieldType(4)
case object UInt48Field extends FixedSizeFieldType(6)
case object SInt16Field extends FixedSizeFieldType(2)
case object SInt32Field extends FixedSizeFieldType(4)
case object Float32Field extends FixedSizeFieldType(4)
case object Float64Field extends FixedSizeFieldType(8)
sealed case class EnumFieldType(model: EnumModel) extends FixedSizeFieldType(1) {
  override def defaultValue: String = "%s::%s".format(model.name, model.default.displayName)
}

object FixedSizeField {

  //common flags field
  val flags = FixedSizeField("flags", UInt8Field)


  // SA stuff
  val csq = FixedSizeField("challengeSeqNum", UInt32Field)
  val ksq = FixedSizeField("keyChangeSeqNum", UInt32Field)
  val user = FixedSizeField("userNum", UInt16Field)
  val assocId = FixedSizeField("assocId", UInt16Field)
  val macAlgo = FixedSizeField("hmacAlgo", EnumFieldType(HMACType()))
  val keyWrapAlgo = FixedSizeField("keyWrapAlgo", EnumFieldType(KeyWrapAlgorithm()))
  val keyStatus = FixedSizeField("keyStatus", EnumFieldType(KeyStatus()))
  val challengeReason = FixedSizeField("challengeReason", EnumFieldType(ChallengeReason()))
  val errorCode = FixedSizeField("errorCode", EnumFieldType(AuthErrorCode()))
  val keyChangeMethod = FixedSizeField("keyChangeMethod", EnumFieldType(KeyChangeMethod()))
  val certificateType = FixedSizeField("certificateType", EnumFieldType(CertificateType()))


  // timestamps
  val time16 = FixedSizeField("time", UInt16Field)
  val time48 = FixedSizeField("time", UInt48Field)

  // counter values
  val count16 = FixedSizeField("value", UInt16Field)
  val count32 = FixedSizeField("value", UInt32Field)

  // analog values
  val value16 = FixedSizeField("value", SInt16Field)
  val value32 = FixedSizeField("value", SInt32Field)
  val float32 = FixedSizeField("value", Float32Field)
  val float64 = FixedSizeField("value", Float64Field)

  //enums
  val commandStatus = FixedSizeField("status", UInt8Field)
  val intervalUnit = FixedSizeField("intervalUnit", EnumFieldType(IntervalUnit()))

}

object VariableFields {
  val challengeData = VariableField("challengeData")
  val hmac = VariableField("hmacValue")
  val keyWrapData = VariableField("keyWrapData")
  val errorText = VariableField("errorText")
  val certificate = VariableField("certificate")
}

sealed trait Field {
  def name: String
  def cppType: String
  def cppArgument: String = cppType
}

sealed case class FixedSizeField(name: String, typ: FixedSizeFieldType) extends Field {

  def cppType: String = typ match {
    case UInt8Field => "uint8_t"
    case UInt16Field => "uint16_t"
    case UInt32Field => "uint32_t"
    case UInt48Field => "DNPTime"
    case SInt16Field => "int16_t"
    case SInt32Field => "int32_t"
    case Float32Field => "float"
    case Float64Field => "double"
    case EnumFieldType(model: EnumModel) => model.name
  }

}
sealed case class VariableField(name: String) extends Field {
  def cppType: String = "openpal::ReadBufferView"
  override def cppArgument = "const openpal::ReadBufferView&"
}


