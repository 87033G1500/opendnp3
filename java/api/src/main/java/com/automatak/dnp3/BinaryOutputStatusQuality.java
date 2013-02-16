/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;

import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

/**
 * Quality enumeration for BinaryOutputStatus values
 */
public enum BinaryOutputStatusQuality implements QualityField {
    /**
     * Set when the data is "good", meaning that rest of the system can trust the value
     */
    ONLINE((byte) 0x01),

    /**
     * the quality all points get before we have established communication (or populated) the point in a remote database
     */
    RESTART((byte) 0x02),

    /**
     *  Communication has been lost with the source of the data (after establishing contact)
     */
    COMM_LOST((byte) 0x04),

    /**
     *   the value is being forced to a "fake" value somewhere in the system
     */
    REMOTE_FORCED_DATA((byte) 0x08),

    /**
     * the value is being forced to a "fake" value on the original device
     */
    LOCAL_FORCED_DATA((byte) 0x10),

    /**
     * set when the value is oscillating very quickly and some events are being suppressed
     */
    RESERVED_1((byte) 0x20),

    /**
     * reserverd value
     */
    RESERVED_2((byte) 0x40),

    /**
     * the actual value of the binary
     */
    STATE((byte) 0x80);

    private final byte id;

    private BinaryOutputStatusQuality(byte id)
    {
        this.id = id;
    }

    public static Set<BinaryOutputStatusQuality> getValuesInBitField(byte bitfield)
    {
        Set<BinaryOutputStatusQuality> set = new HashSet<BinaryOutputStatusQuality>();
        if((bitfield & ONLINE.toByte()) != 0)  set.add(ONLINE);
        if((bitfield & RESTART.toByte()) != 0)  set.add(RESTART);
        if((bitfield & COMM_LOST.toByte()) != 0)  set.add(COMM_LOST);
        if((bitfield & REMOTE_FORCED_DATA.toByte()) != 0) set.add(REMOTE_FORCED_DATA);
        if((bitfield & LOCAL_FORCED_DATA.toByte()) != 0) set.add(LOCAL_FORCED_DATA);
        if((bitfield & RESERVED_1.toByte()) != 0) set.add(RESERVED_1);
        if((bitfield & RESERVED_2.toByte()) != 0) set.add(RESERVED_2);
        if((bitfield & STATE.toByte()) != 0) set.add(STATE);
        return Collections.unmodifiableSet(set);
    }

    /**
     * @return The underlying integer representation of the enum
     */
    @Override
    public byte toByte()
    {
        return id;
    }
}
