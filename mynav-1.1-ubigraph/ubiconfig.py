
ubi_graph = None
ubi_is_initialized = False

# ubigraph server XMLRPC URL
ubi_server_url = "http://192.168.254.100:20738/RPC2"

# ubigraph client IP address 
# i.e., host that runs IDA
ubi_local_addr = "192.168.254.1"

#
# If you're using IDA and UbiGraph server on the
# same machine - just write 127.0.0.1 in both of
# ubi_server_url and ubi_local_addr
#

##############################################
# variables and other stuff for internal use
##############################################

try:
    
    # initialize XMLRPC server for UbiGraph vertex callbacks
    import ubicallback
    ubicallback.init()

except:

    pass
