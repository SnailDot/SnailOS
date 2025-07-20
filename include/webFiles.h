#ifndef __WEB_FILES_H
#define __WEB_FILES_H

// HTML file
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>SnailOS WebGUI</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f0f0f0;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background-color: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        h1 {
            color: #333;
            text-align: center;
        }
        .status {
            margin: 20px 0;
            padding: 15px;
            background-color: #e8f5e9;
            border-radius: 4px;
        }
        .button {
            background-color: #4CAF50;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            margin: 5px;
            width: 200px;
            display: block;
            text-align: center;
            font-size: 16px;
        }
        .button:hover {
            background-color: #45a049;
        }
        .button.danger {
            background-color: #f44336;
        }
        .button.danger:hover {
            background-color: #da190b;
        }
        .button-container {
            display: flex;
            flex-direction: column;
            align-items: center;
            gap: 10px;
            margin-top: 20px;
        }
        /* File System UI Styles */
        #fileSystem {
            display: none;
            margin-top: 20px;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 15px;
        }
        #fileSystem.active {
            display: block;
        }
        .file-browser {
            margin-top: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 10px;
            max-height: 400px;
            overflow-y: auto;
        }
        .file-table {
            width: 100%;
            border-collapse: collapse;
        }
        .file-table th, .file-table td {
            padding: 8px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }
        .file-table th {
            background-color: #f5f5f5;
        }
        .file-row:hover {
            background-color: #f9f9f9;
        }
        .file-actions {
            display: flex;
            gap: 10px;
            margin-top: 10px;
        }
        .file-actions button {
            padding: 5px 10px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 14px;
        }
        .file-actions .upload-btn {
            background-color: #2196F3;
            color: white;
        }
        .file-actions .new-folder-btn {
            background-color: #FF9800;
            color: white;
        }
        .file-icon {
            margin-right: 5px;
            color: #FFC107;
        }
        .file-size {
            color: #666;
            font-size: 0.9em;
        }
        .upload-progress {
            margin-top: 10px;
            display: none;
        }
        .upload-progress.active {
            display: block;
        }
        .progress-bar {
            width: 100%;
            height: 20px;
            background-color: #f0f0f0;
            border-radius: 10px;
            overflow: hidden;
        }
        .progress-bar-fill {
            height: 100%;
            background-color: #4CAF50;
            width: 0%;
            transition: width 0.3s ease;
        }
        #drop-area {
            border: 2px dashed #ccc;
            border-radius: 4px;
            padding: 20px;
            text-align: center;
            margin-top: 10px;
            display: none;
        }
        #drop-area.highlight {
            border-color: #4CAF50;
            background-color: #e8f5e9;
        }
        .status-message {
            margin-top: 10px;
            padding: 10px;
            border-radius: 4px;
            display: none;
        }
        .status-message.success {
            background-color: #e8f5e9;
            color: #2e7d32;
            display: block;
        }
        .status-message.error {
            background-color: #ffebee;
            color: #c62828;
            display: block;
        }
        /* Firmware Store UI Styles */
        #firmwareStore {
            display: none;
            margin-top: 20px;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 15px;
        }
        #firmwareStore.active {
            display: block;
        }
        .store-categories {
            display: flex;
            flex-direction: row;
            gap: 10px;
            margin-bottom: 20px;
            flex-wrap: wrap;
        }
        .store-category {
            background-color: #4CAF50;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            min-width: 120px;
            text-align: center;
        }
        .store-category:hover {
            background-color: #45a049;
        }
        .store-content {
            display: flex;
            gap: 20px;
        }
        .firmware-list {
            display: none;
            flex: 1;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 15px;
            background-color: #f8f9fa;
        }
        .firmware-list.active {
            display: block;
        }
        .firmware-item {
            background-color: white;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 15px;
            margin: 10px 0;
            cursor: pointer;
        }
        .firmware-item:hover {
            background-color: #e9ecef;
        }
        .firmware-details {
            display: none;
            margin-top: 20px;
            padding: 15px;
            background-color: #f8f9fa;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
        .firmware-details.active {
            display: block;
        }
        .download-button {
            background-color: #007bff;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            margin-top: 10px;
        }
        .download-button:hover {
            background-color: #0056b3;
        }
        .back-button {
            background-color: #6c757d;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            margin-top: 10px;
        }
        .back-button:hover {
            background-color: #5a6268;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>SnailOS WebGUI</h1>
        <div class="status">
            <h2>System Status</h2>
            <p>IP Address: <span id="ip">Loading...</span></p>
            <p>Status: <span id="status">Loading...</span></p>
        </div>
        <div class="button-container">
            <button class="button" onclick="toggleFileSystem()">Files</button>
            <button class="button" onclick="openDeviceStatus()">Device Status</button>
        </div>

        <!-- File System UI -->
        <div id="fileSystem">
            <h2>File System</h2>
            <div class="file-actions">
                <button class="upload-btn" onclick="showUploadArea()">Upload Files</button>
                <button class="new-folder-btn" onclick="createNewFolder()">New Folder</button>
            </div>
            <div id="drop-area">
                <p>Drag & Drop files here or click to select</p>
                <input type="file" id="fileInput" multiple style="display: none">
            </div>
            <div class="upload-progress">
                <div class="progress-bar">
                    <div class="progress-bar-fill"></div>
                </div>
                <p id="upload-status">Uploading...</p>
            </div>
            <div id="status-message" class="status-message"></div>
            <div class="file-browser">
                <table class="file-table">
                    <thead>
                        <tr>
                            <th>Name</th>
                            <th>Size</th>
                            <th>Actions</th>
                        </tr>
                    </thead>
                    <tbody id="file-list"></tbody>
                </table>
            </div>
        </div>
    </div>

    <script>
        // Global variables
        let currentFolder = "/";
        let isFileSystemVisible = false;
        let currentFirmware = null;

        // Toggle file system visibility
        function toggleFileSystem() {
            const fileSystem = document.getElementById('fileSystem');
            isFileSystemVisible = !isFileSystemVisible;
            
            // Toggle file system
            fileSystem.classList.toggle('active');
            
            if (isFileSystemVisible) {
                listFiles(currentFolder);
            }
        }

        // List files in current directory
        function listFiles(folder) {
            currentFolder = folder;
            fetch('/listfiles?folder=' + encodeURIComponent(folder))
                .then(response => response.text())
                .then(data => {
                    const lines = data.split('\n');
                    const fileList = document.getElementById('file-list');
                    fileList.innerHTML = '';
                    
                    // Add parent directory link
                    if (folder !== '/') {
                        const parentFolder = folder.substring(0, folder.lastIndexOf('/'));
                        const parentRow = createFileRow('..', 'folder', '0', parentFolder);
                        fileList.appendChild(parentRow);
                    }
                    
                    // Process each file/folder
                    lines.forEach(line => {
                        if (!line) return;
                        const [type, name, size] = line.split(':');
                        if (type === 'Fo' || type === 'Fi') {
                            const path = folder + (folder.endsWith('/') ? '' : '/') + name;
                            const row = createFileRow(name, type === 'Fo' ? 'folder' : 'file', size, path);
                            fileList.appendChild(row);
                        }
                    });
                })
                .catch(error => {
                    showStatus('Error loading files: ' + error, 'error');
                });
        }

        // Create a file/folder row
        function createFileRow(name, type, size, path) {
            const row = document.createElement('tr');
            row.className = 'file-row';
            
            // Name column
            const nameCell = document.createElement('td');
            const icon = document.createElement('span');
            icon.className = 'file-icon';
            icon.textContent = type === 'folder' ? '📁' : '📄';
            nameCell.appendChild(icon);
            
            if (type === 'folder') {
                const link = document.createElement('a');
                link.href = '#';
                link.textContent = name;
                link.onclick = (e) => {
                    e.preventDefault();
                    listFiles(path);
                };
                nameCell.appendChild(link);
            } else {
                nameCell.appendChild(document.createTextNode(name));
            }
            
            // Size column
            const sizeCell = document.createElement('td');
            sizeCell.className = 'file-size';
            sizeCell.textContent = type === 'folder' ? '' : formatSize(size);
            
            // Actions column
            const actionsCell = document.createElement('td');
            const actionsDiv = document.createElement('div');
            actionsDiv.className = 'file-actions';
            
            if (type === 'file') {
                const downloadBtn = document.createElement('button');
                downloadBtn.textContent = 'Download';
                downloadBtn.onclick = () => downloadFile(path);
                actionsDiv.appendChild(downloadBtn);
            }
            
            const renameBtn = document.createElement('button');
            renameBtn.textContent = 'Rename';
            renameBtn.onclick = () => renameFile(path, name);
            actionsDiv.appendChild(renameBtn);
            
            const deleteBtn = document.createElement('button');
            deleteBtn.textContent = 'Delete';
            deleteBtn.className = 'danger';
            deleteBtn.onclick = () => deleteFile(path);
            actionsDiv.appendChild(deleteBtn);
            
            actionsCell.appendChild(actionsDiv);
            
            row.appendChild(nameCell);
            row.appendChild(sizeCell);
            row.appendChild(actionsCell);
            
            return row;
        }

        // File operations
        function downloadFile(path) {
            window.open('/file?name=' + encodeURIComponent(path) + '&action=download', '_blank');
        }

        function deleteFile(path) {
            if (confirm('Are you sure you want to delete: ' + path + '?')) {
                fetch('/file?name=' + encodeURIComponent(path) + '&action=delete')
                    .then(response => response.text())
                    .then(data => {
                        showStatus(data, 'success');
                        listFiles(currentFolder);
                    })
                    .catch(error => {
                        showStatus('Error deleting file: ' + error, 'error');
                    });
            }
        }

        function renameFile(path, oldName) {
            const newName = prompt('Enter new name:', oldName);
            if (newName && newName !== oldName) {
                const formData = new FormData();
                formData.append('filePath', path);
                formData.append('fileName', newName);
                
                fetch('/rename', {
                    method: 'POST',
                    body: formData
                })
                .then(response => response.text())
                .then(data => {
                    showStatus(data, 'success');
                    listFiles(currentFolder);
                })
                .catch(error => {
                    showStatus('Error renaming file: ' + error, 'error');
                });
            }
        }

        function createNewFolder() {
            const folderName = prompt('Enter folder name:');
            if (folderName) {
                const path = currentFolder + (currentFolder.endsWith('/') ? '' : '/') + folderName;
                fetch('/file?name=' + encodeURIComponent(path) + '&action=create')
                    .then(response => response.text())
                    .then(data => {
                        showStatus(data, 'success');
                        listFiles(currentFolder);
                    })
                    .catch(error => {
                        showStatus('Error creating folder: ' + error, 'error');
                    });
            }
        }

        // Upload handling
        function showUploadArea() {
            const dropArea = document.getElementById('drop-area');
            dropArea.style.display = dropArea.style.display === 'none' ? 'block' : 'none';
            if (dropArea.style.display === 'block') {
                document.getElementById('fileInput').click();
            }
        }

        // Drag and drop handling
        const dropArea = document.getElementById('drop-area');
        const fileInput = document.getElementById('fileInput');

        ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
            dropArea.addEventListener(eventName, preventDefaults, false);
        });

        function preventDefaults(e) {
            e.preventDefault();
            e.stopPropagation();
        }

        ['dragenter', 'dragover'].forEach(eventName => {
            dropArea.addEventListener(eventName, highlight, false);
        });

        ['dragleave', 'drop'].forEach(eventName => {
            dropArea.addEventListener(eventName, unhighlight, false);
        });

        function highlight(e) {
            dropArea.classList.add('highlight');
        }

        function unhighlight(e) {
            dropArea.classList.remove('highlight');
        }

        dropArea.addEventListener('drop', handleDrop, false);
        fileInput.addEventListener('change', handleFiles, false);

        function handleDrop(e) {
            const dt = e.dataTransfer;
            const files = dt.files;
            handleFiles({ target: { files: files } });
        }

        function handleFiles(e) {
            const files = [...e.target.files];
            uploadFiles(files);
        }

        function uploadFiles(files) {
            const progressBar = document.querySelector('.progress-bar-fill');
            const uploadStatus = document.getElementById('upload-status');
            const uploadProgress = document.querySelector('.upload-progress');
            
            uploadProgress.classList.add('active');
            let uploaded = 0;
            
            files.forEach((file, index) => {
                const formData = new FormData();
                formData.append('file', file);
                formData.append('folder', currentFolder);
                
                const xhr = new XMLHttpRequest();
                xhr.upload.addEventListener('progress', (e) => {
                    if (e.lengthComputable) {
                        const percent = (uploaded + (e.loaded / e.total)) / files.length * 100;
                        progressBar.style.width = percent + '%';
                        uploadStatus.textContent = `Uploading ${index + 1} of ${files.length}: ${file.name}`;
                    }
                });
                
                xhr.addEventListener('load', () => {
                    uploaded++;
                    if (uploaded === files.length) {
                        uploadProgress.classList.remove('active');
                        showStatus('Upload complete', 'success');
                        listFiles(currentFolder);
                    }
                });
                
                xhr.addEventListener('error', () => {
                    showStatus('Upload failed: ' + file.name, 'error');
                });
                
                xhr.open('POST', '/');
                xhr.send(formData);
            });
        }

        // Utility functions
        function formatSize(bytes) {
            if (bytes === '0') return '0 B';
            const units = ['B', 'KB', 'MB', 'GB'];
            let size = parseFloat(bytes);
            let unit = 0;
            while (size >= 1024 && unit < units.length - 1) {
                size /= 1024;
                unit++;
            }
            return size.toFixed(1) + ' ' + units[unit];
        }

        function showStatus(message, type) {
            const statusElement = document.getElementById('status-message');
            statusElement.textContent = message;
            statusElement.className = 'status-message ' + type;
            setTimeout(() => {
                statusElement.className = 'status-message';
            }, 3000);
        }

        // System info update
        function updateSystemInfo() {
            fetch('/systeminfo')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('ip').textContent = data.ip;
                    document.getElementById('status').textContent = data.status;
                })
                .catch(error => {
                    console.error('Error:', error);
                    document.getElementById('status').textContent = 'Error fetching status';
                });
        }

        // Placeholder functions for other buttons
        function openDeviceStatus() {
            alert('Device status page coming soon!');
        }

        // Initialize
        updateSystemInfo();
        setInterval(updateSystemInfo, 5000);
    </script>
</body>
</html>
)rawliteral";

const size_t index_html_size = sizeof(index_html);

#endif // __WEB_FILES_H 