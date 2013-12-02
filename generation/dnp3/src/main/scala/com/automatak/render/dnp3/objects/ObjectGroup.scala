package com.automatak.render.dnp3.objects

import com.automatak.render.dnp3.objects.groups._

object ObjectGroup {

 val all = List(
    Group1,
    Group2,
    Group3,
    Group4,
    Group10,
    Group11,
    Group12,
    Group20
  )

}

trait ObjectGroup {
  def objects: List[GroupVariation]
  def group: Byte
}
