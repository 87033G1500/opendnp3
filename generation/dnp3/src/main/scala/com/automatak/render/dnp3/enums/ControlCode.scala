package com.automatak.render.dnp3.enums

import com.automatak.render._


object ControlCode {

  private val comments = List(
    "There are a number of types of controls. The best way to understand this ",
    "difference is to think about the hardware controls the communication protocols are ",
    "emulating. The most common to use are PULSE_ON, LATCH_ON and LATCH_OFF",
    "",
    "NOTE: Current implementation doesn't support queue/clear.",
    "",
    "An enumeration of result codes received from an outstation in response to command request.",
    "These correspond to those defined in the DNP3 standard"
  )

  def apply(): EnumModel = EnumModel("ControlCode", comments, EnumModel.UInt8, codes, Hex)

  private val codes = List(
    EnumValue("NUL", 0x00, "illegal command code (used internally)"),
    EnumValue("PULSE_ON", 0x01, "a 'push-button' interface, can only be pressed one way (reset button on pedometer)"),
    EnumValue("PULSE_OFF", 0x02, "a 'push-button' interface with output remaining active following pulse"),
    EnumValue("LATCH_ON", 0x03, "a 'light-switch' moved to the ON position"),
    EnumValue("LATCH_OFF", 0x04, "a 'light-switch' moved to the OFF position"),
    EnumValue("PULSE_CLOSE", 0x41, " a 'doorbell' that rings while the button is depressed"),
    EnumValue("PULSE_TRIP", 0x81, " a 'doorbell' that stops ringing (is normally on) while depressed"),
    EnumValue("UNDEFINED", 0xFF, " undefined command (used by DNP standard)")
  )

}



