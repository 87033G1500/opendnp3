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
package com.automatak.dnp3.example;

import com.automatak.dnp3.*;

public class FormattingDataObserver implements DataObserver {

    private final OutputHandler handler;

    public FormattingDataObserver(OutputHandler handler)
    {
        this.handler = handler;
    }

    public void start()
    {
        handler.handleOutput("start");
    }

    public void update(BinaryInput meas, long index)
    {
        handler.handleOutput("Binary: " + meas.getValue());
    }

    public void update(AnalogInput meas, long index)
    {
        handler.handleOutput("Analog: " + meas.getValue());
    }

    public void update(Counter meas, long index)
    {
        handler.handleOutput("Counter: " + meas.getValue());
    }

    public void update(BinaryOutputStatus meas, long index)
    {
        handler.handleOutput("BinaryOutputStatus: " + meas.getValue());
    }

    public void update(AnalogOutputStatus meas, long index)
    {
        handler.handleOutput("AnalogOutputStatus: " + meas.getValue());
    }

    public void end()
    {
        handler.handleOutput("end");
    }
}
