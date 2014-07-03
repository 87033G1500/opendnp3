package com.automatak.render.dnp3.enums

import com.automatak.render._


object PollState {

  private val comments = List("Enumeration for the state of a poll")

  def apply(): EnumModel = EnumModel("PollState", comments, EnumModel.Integer, codes, Base10)

  private val codes = List(
       EnumValue("RUNNING", 0, None,  "Valid response was received"),
       EnumValue("SUCCESS", 1, None,  "Valid response was received"),
       EnumValue("FAILURE", 2, None,  "The operation timed out or explicitly failed")
  )

}



