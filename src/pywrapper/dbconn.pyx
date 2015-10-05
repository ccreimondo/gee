from thrift.transport import TSocket
from thrift.protocol import TBinaryProtocol
from thrift.transport import TTransport
from hbase import Hbase

host = "127.0.0.1"
port = 9095

transport = TTransport.TBufferedTransport(TSocket.TSocket(host, port))
protocol = TBinaryProtocol.TBinaryProtocol(transport)

client = Hbase.Client(protocol)
transport.open()

cdef public py_print(const char *str):
    print str


cdef public init_table():
    client.createTable("py_test", [Hbase.ColumnDescriptor(name="new")])
    print "init success"


cdef public has_table(const char *table_name):
    tables = client.getTableNames()
    found = False
    for table in tables:
        if table == table_name:
            found = True

    if found:
        print "Table: %s exits." % table_name
    else:
        print "Table: %s not exits." % table_name


transport.close()