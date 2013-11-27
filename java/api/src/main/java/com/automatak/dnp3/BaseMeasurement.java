/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;

/**
 * Base class for all measurements that provides time and quality bit-fields
 */
public abstract class BaseMeasurement implements Measurement {

    private final byte quality;
    private final long timestamp;

    public BaseMeasurement(byte quality, long timestamp)
    {
        this.quality = quality;
        this.timestamp = timestamp;
    }

    /**
     * @return milliseconds since Unix epoch UTC
     */
    public long getMsSinceEpoch()
    {
        return timestamp;
    }

    /**
     * @return bit-field representing measurement quality
     */
    public byte getQuality()
    {
        return quality;
    }

    @Override
    public String toString()
    {
        StringBuilder s = new StringBuilder();
        s.append(this.getClass().getSimpleName());
        s.append(" - ");
        s.append(this.getValueAsString());
        s.append(" - ");
        s.append(this.getQualityAsString());
        return s.toString();
    }

}
