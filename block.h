class Block
{
    private:  
    int tag;  
    bool dirty;

    public:
    Block();
    Block(int c);
    Block(int c, bool d);
    int getTag();
    bool isDirty();
    void setDirty();
    ~Block()
    {
        // delete this;
    }
};