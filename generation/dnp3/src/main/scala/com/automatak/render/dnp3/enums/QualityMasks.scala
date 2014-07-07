package com.automatak.render.dnp3.enums

import com.automatak.render._

object QualityMasks {

  private def comments(name: String) = List("Quality field bitmask for " + name + " values.")


  def enums: List[EnumModel] = List(
    EnumModel("BinaryQuality", comments("binary"), EnumModel.UInt8, binaryValues, Hex),
    EnumModel("DoubleBitBinaryQuality", comments("double bit binary"), EnumModel.UInt8, doubleBinaryValues, Hex),
    EnumModel("CounterQuality", comments("counter"), EnumModel.UInt8, counterValues, Hex),
    EnumModel("AnalogQuality", comments("analog"), EnumModel.UInt8, analogValues, Hex),
    EnumModel("BinaryOutputStatusQuality", comments("binary output status"), EnumModel.UInt8, binaryOutputValues, Hex)
  )

  private def bits() : Iterator[Int] = Iterator.iterate(1)(x => x << 1)

  private val online = "ONLINE" -> "set when the data is \"good\", meaning that rest of the system can trust the value"
  private val restart = "RESTART" -> "the quality all points get before we have established communication (or populated) the point"
  private val commsLost = "COMM_LOST" -> "set if communication has been lost with the source of the data (after establishing contact)"
  private val remoteForedData = "REMOTE_FORCED_DATA" -> "set if the value is being forced to a \"fake\" value somewhere in the system"
  private val localForcedData = "LOCAL_FORCED_DATA" -> "set if the value is being forced to a \"fake\" value on the original device"
  private val overrange = "OVERRANGE" -> "set if a hardware input etc. is out of range and we are using a place holder value"
  private val chatterFilter ="CHATTER_FILTER" ->"set if the value is oscillating very quickly and some events are being suppressed"
  private val state =  "STATE" -> "state bit"
  private val state1 =  "STATE1" -> "state bit 1"
  private val state2 =  "STATE2" -> "state bit 2"
  private val referenceErr = "REFERENCE_ERR" -> "set if calibration or reference voltage has been lost meaning readings are questionable"
  private val rollover = "ROLLOVER" -> "Deprecated flag that indicates value has rolled over"
  private val discontinuity = "DISCONTINUITY" -> "indicates an unusual change in value"

  private val reserved = "RESERVED" -> "reserved bit"

  private val reserved1 = "RESERVED1" -> "reserved bit 1"
  private val reserved2 = "RESERVED2" -> "reserved bit 2"

  private def toValues(list : List[(String, String)]): List[EnumValue] = {
    val bit : Iterator[Int] = bits()
    list.map(x => EnumValue(x._1, bit.next(), Some(x._2)))
  }

  private val binaryValues = toValues(
    List(
      online,
      restart,
      commsLost,
      remoteForedData,
      localForcedData,
      chatterFilter,
      reserved,
      state
    )
  )

  private val doubleBinaryValues = toValues(
    List(
      online,
      restart,
      commsLost,
      remoteForedData,
      localForcedData,
      chatterFilter,
      state1,
      state2
    )
  )

  private val counterValues = toValues(
    List(
      online,
      restart,
      commsLost,
      remoteForedData,
      localForcedData,
      rollover,
      discontinuity,
      reserved
    )
  )

  private val analogValues = toValues(
    List(
      online,
      restart,
      commsLost,
      remoteForedData,
      localForcedData,
      overrange,
      referenceErr,
      reserved
    )
  )

  private val binaryOutputValues = toValues(
    List(
      online,
      restart,
      commsLost,
      remoteForedData,
      localForcedData,
      reserved1,
      reserved2,
      state
    )
  )

}



