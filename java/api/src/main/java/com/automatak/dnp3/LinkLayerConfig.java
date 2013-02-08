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

/**
 * Configuration class for the link layer
 */
public class LinkLayerConfig {

    /**
     * Controls the master/slave bit set on all messages
     */
    public boolean isMaster;

    /**
     * If true, the link layer will send data requesting confirmation. This is generally NEVER
     * set for a TCP connection and only sometimes set for serial.
     *
     * defaults to false
     */
    public boolean useConfirms = false;

    /**
     * The number of retry attempts the link will attempt after the initial try if using confirms
     *
     * defaults to 0
     */
    public int numRetry = 0;

    /**
     * dnp3 address of the local device as a 16-bit unsigned integer
     */
    public int localAddr;

    /**
     * dnp3 address of the remote device as a 16-bit unsigned integer
     */
    public int remoteAddr;

    /**
     * the response timeout in milliseconds for confirmed requests
     */
    public long timeoutMs = 1000;

    private LinkLayerConfig()
    {

    }

    /**
     * Constructor that sets reasonable defaults. Override after construction
     * @param isMaster True if the link layer is for a master, false for an outstation
     */
    public LinkLayerConfig(boolean isMaster)
    {
        this.isMaster = isMaster;
        this.localAddr = isMaster ? 1 : 1024;
        this.remoteAddr = isMaster ? 1024: 1;
    }

}
